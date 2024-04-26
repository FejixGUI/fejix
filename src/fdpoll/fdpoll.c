#include <src/shared/fdpoll/fdpoll.h>

#include <fejix/malloc.h>
#include <fejix/utils.h>

#include <unistd.h>


static
fj_err_t process_events(
    void * callback_data,
    struct fj_fdpoll * poller
)
{
    FJ_INIT_ERRORS

    struct pollfd * pollfds = FJ_VEC_OF(poller->pollfds, struct pollfd);

    fj_fdpoll_callback_fn_t * *fjARRAY callbacks = FJ_VEC_OF(
        poller->callbacks,
        fj_fdpoll_callback_fn_t *
    );

    for (uint32_t i = 0; i < poller->pollfds.length; i++) {

        struct pollfd * pollfd = &pollfds[i];

        fj_fdpoll_callback_fn_t * callback = callbacks[i];

        if (pollfd->revents == 0) {
            continue;
        }

        FJ_TRY callback(callback_data, pollfd->fd, pollfd->revents);

        if (FJ_FAILED) {
            return FJ_LAST_ERROR;
        }

        pollfd->revents = 0;
    }

    return FJ_OK;
}


static
fj_err_t process_interruption(
    void * callback_data,
    fj_fdpoll_fd_t fd,
    fj_fdpoll_event_mask_t events
)
{
    FJ_INTEND_UNUSED(callback_data)

    if (events & (POLLERR|POLLHUP|POLLNVAL)) {
        return FJ_RESULT("error on interruptor FD");
    }

    uint8_t buffer[1];
    read(fd, buffer, 1);

    return FJ_OK;
}


fj_err_t fj_fdpoll_init(struct fj_fdpoll * poller)
{
    FJ_INIT_ERRORS

    fj_vec_init(&poller->pollfds, sizeof(struct pollfd));
    fj_vec_init(&poller->callbacks, sizeof(fj_fdpoll_callback_fn_t *));

    int pipe_result = pipe((int32_t *) poller->interrupt_pipe);

    if (pipe_result < 0) {
        return FJ_RESULT("fdpoll pipe failed");
    }

    FJ_TRY fj_fdpoll_add(
        poller,
        fj_fdpoll_get_interruptor(poller),
        POLLIN,
        process_interruption
    );

    if (FJ_FAILED) {
        fj_fdpoll_deinit(poller);
        return FJ_LAST_ERROR;
    }

    return FJ_OK;
}


void fj_fdpoll_deinit(struct fj_fdpoll * poller)
{
    if (poller->interrupt_pipe[0] != 0) {
        close(poller->interrupt_pipe[0]);
        close(poller->interrupt_pipe[1]);
    }

    fj_vec_deinit(&poller->pollfds);
    fj_vec_deinit(&poller->callbacks);
}


fj_err_t fj_fdpoll_add(
    struct fj_fdpoll * poller,
    fj_fdpoll_fd_t file_descriptor,
    fj_fdpoll_event_mask_t events_to_watch,
    fj_fdpoll_callback_fn_t * callback
)
{
    FJ_INIT_ERRORS

    struct pollfd pollfd = {
        .fd = file_descriptor,
        .events = events_to_watch,
        .revents = 0,
    };

    FJ_TRY fj_vec_push_item(&poller->pollfds, &pollfd);

    if (FJ_FAILED) {
        return FJ_LAST_ERROR;
    }

    FJ_TRY fj_vec_push_item(&poller->callbacks, &callback);

    if (FJ_FAILED) {
        return FJ_LAST_ERROR;
    }

    return FJ_OK;
}


static
fj_bool_t search_pollfd(void * data, void * item)
{
    return ((struct pollfd *) item)->fd == * (fj_fdpoll_fd_t *) data;
}

fj_err_t fj_fdpoll_remove(
    struct fj_fdpoll * poller,
    fj_fdpoll_fd_t file_descriptor
)
{
    FJ_INIT_ERRORS

    uint32_t index = fj_vec_find(
        &poller->pollfds,
        search_pollfd,
        &file_descriptor
    );

    if (index == FJ_VEC_LAST_INDEX(poller->pollfds)) {
        return FJ_OK;
    }

    FJ_TRY fj_vec_remove_items(&poller->pollfds, index, 1);

    if (FJ_FAILED) {
        return FJ_LAST_ERROR;
    }

    return fj_vec_remove_items(&poller->callbacks, index, 1);
}


static
int32_t convert_poll_timeout(uint32_t timeout)
{
    if (timeout == UINT32_MAX) {
        return -1;
    }

    if (timeout > UINT16_MAX) {
        return UINT16_MAX;
    }

    return (int32_t) timeout;
}

fj_err_t fj_fdpoll_poll(
    struct fj_fdpoll * poller,
    uint32_t timeout_ms,
    void * callback_data
)
{
    int32_t result = poll(
        poller->pollfds.items,
        poller->pollfds.length,
        convert_poll_timeout(timeout_ms)
    );

    if (result < 0) {
        return FJ_RESULT("fdpoll poll failed");
    }

    if (result == 0) {
        return FJ_OK;
    }

    return process_events(callback_data, poller);
}


fj_fdpoll_fd_t fj_fdpoll_get_interruptor(
    struct fj_fdpoll * poller
)
{
    return poller->interrupt_pipe[1];
}


fj_err_t fj_fdpoll_interrupt(
    fj_fdpoll_fd_t interruptor
)
{
    uint8_t buffer[1] = { 42 };
    ssize_t written_count = write(interruptor, buffer, 1);

    if (written_count < 0) {
        return FJ_RESULT("fdpoll failed to write to interruptor FD");
    }

    return FJ_OK;
}
