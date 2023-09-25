#include <src/unixpoller/unixpoller.h>

#include <fejix/list.h>
#include <fejix/malloc.h>
#include <fejix/utils.h>

#include <unistd.h>


struct fj_unixpoller {
    fj_unixpoller_fd_t interrupt_pipe[2];
    struct fj_list * pollfds;
    struct fj_list * callbacks;
};


static fj_err_t unixpoller_process_events(
    struct fj_client * client,
    struct fj_unixpoller * poller
)
{
    for (uint32_t i=0; i<poller->pollfds->length; i++) {
        struct pollfd * pollfd = fj_list_get(poller->pollfds, i);

        fj_unixpoller_callback_t callback
            = * (fj_unixpoller_callback_t *) fj_list_get(poller->callbacks, i);

        if (pollfd->revents != 0) {
            // TODO does callback need to handle POLLNVAL (invalid FD error)?
            fj_err_t err = callback(client, pollfd->fd, pollfd->revents);
            if (err != FJ_OK) return err;

            pollfd->revents = 0;
        }
    }

    return FJ_OK;
}


static fj_err_t unixpoller_process_interruption(
    struct fj_client * client,
    fj_unixpoller_fd_t fd,
    fj_unixpoller_event_mask_t events
)
{
    (void) client;

    if (events & (POLLERR|POLLHUP|POLLNVAL)) {
        return FJ_ERR("error on interruptor FD");
    }

    uint8_t buffer[1];
    read(fd, buffer, 1);

    return FJ_OK;
}


static fj_err_t unixpoller_init(struct fj_unixpoller * poller)
{
    poller->pollfds = fj_list_new(sizeof(struct pollfd));
    poller->callbacks = fj_list_new(sizeof(fj_unixpoller_callback_t));

    if (poller->pollfds || poller->callbacks == NULL) {
        return FJ_ERR(FJ_MALLOC_FAILED);
    }

    int pipe_result = pipe((int32_t *) poller->interrupt_pipe);

    if (pipe_result < 0) {
        return FJ_ERR("pipe failed");
    }

    return fj_unixpoller_watch(
        poller,
        fj_unixpoller_get_interruptor(poller),
        POLLIN,
        unixpoller_process_interruption
    );
}


struct fj_unixpoller * fj_unixpoller_new(void)
{
    struct fj_unixpoller * poller = fj_alloc_zeroed(sizeof *poller);

    if (poller == NULL) {
        return NULL;
    }

    fj_err_t err = unixpoller_init(poller);

    if (err != FJ_OK) {
        fj_unixpoller_del(poller);
        return NULL;
    }

    return poller;
}


void fj_unixpoller_del(struct fj_unixpoller * poller)
{
    if (poller->interrupt_pipe[0] != 0) {
        close(poller->interrupt_pipe[0]);
        close(poller->interrupt_pipe[1]);
    }

    if (poller->callbacks != NULL) {
        fj_free(poller->callbacks);
    }

    if (poller->pollfds != NULL) {
        fj_free(poller->pollfds);
    }

    fj_free(poller);
}


fj_err_t fj_unixpoller_watch(
    struct fj_unixpoller * poller,
    fj_unixpoller_fd_t file_descriptor,
    fj_unixpoller_event_mask_t events_to_watch,
    fj_unixpoller_callback_t callback
)
{
    struct pollfd pollfd = {
        .fd = file_descriptor,
        .events = events_to_watch,
        .revents = 0,
    };

    fj_err_t err = FJ_OK;

    err = fj_list_push(poller->pollfds, &pollfd);
    if (err != FJ_OK) return err;

    err = fj_list_push(poller->callbacks, &callback);
    return err;
}


static fj_bool_t compare_pollfds(fj_ptr_t a, fj_ptr_t b)
{
    return ((struct pollfd *)a)->fd == ((struct pollfd *)b)->fd;
}

fj_err_t fj_unixpoller_unwatch(
    struct fj_unixpoller * poller,
    fj_unixpoller_fd_t file_descriptor
)
{
    struct pollfd pollfd = { .fd = file_descriptor };
    uint32_t index = fj_list_find(poller->pollfds, &pollfd, compare_pollfds);

    if (index == poller->pollfds->length) {
        return FJ_OK;
    }

    fj_err_t err = FJ_OK;

    err = fj_list_remove(poller->pollfds, index);
    if (err != FJ_OK) return err;

    err = fj_list_remove(poller->callbacks, index);
    return err;
}


fj_err_t fj_unixpoller_poll(
    struct fj_unixpoller * poller,
    struct fj_client * client,
    int32_t timeout_ms
)
{
    int32_t result = poll(
        poller->pollfds->elements,
        poller->pollfds->length,
        timeout_ms
    );

    if (result < 0) {
        return FJ_ERR("poll failed");
    }

    if (result == 0) {
        return FJ_OK;
    }

    return unixpoller_process_events(client, poller);
}


fj_unixpoller_fd_t fj_unixpoller_get_interruptor(
    struct fj_unixpoller * poller
)
{
    return poller->interrupt_pipe[1];
}


fj_err_t fj_unixpoller_interrupt(
    fj_unixpoller_fd_t interruptor
)
{
    uint8_t buffer[1] = { 42 };
    ssize_t written_count = write(interruptor, buffer, 1);

    if (written_count < 0) {
        return FJ_ERR("failed to write to interruptor FD");
    }

    return FJ_OK;
}
