#include <src/unixpoller/unixpoller.h>

#include <fejix/core/malloc.h>
#include <fejix/core/utils.h>

#include <math.h>
#include <unistd.h>


static
fj_err_t process_events(struct fj_unixpoller * poller)
{
    FJ_INIT_TRY

    struct pollfd * pollfds = poller->pollfds.items;
    fj_unixpoller_callback_fn_t * * callbacks = poller->callbacks.items;

    for (uint32_t i = 0; i < poller->pollfds.length; i++) {

        if (pollfds[i].revents == 0) {
            continue;
        }

        fj_try callbacks[i](
            poller->callback_data,
            pollfds[i].fd,
            pollfds[i].revents
        );
        fj_else {
            return fj_result;
        }

        pollfds[i].revents = 0;
    }

    return FJ_OK;
}


static
fj_err_t process_interruption(
    void * callback_data,
    fj_unixpoller_fd_t fd,
    fj_unixpoller_event_mask_t events
)
{
    FJ_ARG_UNUSED(callback_data)

    if (events & (POLLERR|POLLHUP|POLLNVAL)) {
        return FJ_ERR_IO_ERROR;
    }

    uint8_t buffer[1];
    read(fd, buffer, 1);

    return FJ_OK;
}


static
fj_err_t interrupt(struct fj_client_interrupt_signal const * _signal)
{
    struct fj_unixpoller_interrupt_signal const * signal = (void *) _signal;

    uint8_t buffer[1] = { 42 };
    ssize_t written_count = write(signal->interrupt_fd, buffer, 1);

    if (written_count < 0) {
        return FJ_ERR_IO_ERROR;
    }

    return FJ_OK;
}



fj_err_t fj_unixpoller_init(struct fj_unixpoller * poller, void * callback_data)
{
    FJ_INIT_TRY

    fj_vec_init(&poller->pollfds, sizeof(struct pollfd));
    fj_vec_init(&poller->callbacks, sizeof(fj_unixpoller_callback_fn_t *));

    int pipe_result = pipe((int32_t *) poller->interrupt_pipe);

    if (pipe_result < 0) {
        return FJ_ERR_IO_ERROR;
    }

    poller->timeout = 0.0;
    poller->callback_data = callback_data;
    poller->interrupt_signal.interrupt_fd = poller->interrupt_pipe[1];
    poller->interrupt_signal.interrupt_signal.interrupt = interrupt;

    fj_try
    fj_unixpoller_add(
        poller,
        poller->interrupt_signal.interrupt_fd,
        POLLIN,
        process_interruption
    );

    fj_else {
        fj_unixpoller_deinit(poller);
        return fj_result;
    }

    return FJ_OK;
}


void fj_unixpoller_deinit(struct fj_unixpoller * poller)
{
    if (poller->interrupt_pipe[0] != 0) {
        close(poller->interrupt_pipe[0]);
        close(poller->interrupt_pipe[1]);
    }

    fj_vec_deinit(&poller->pollfds);
    fj_vec_deinit(&poller->callbacks);
}


fj_err_t fj_unixpoller_add(
    struct fj_unixpoller * poller,
    fj_unixpoller_fd_t file_descriptor,
    fj_unixpoller_event_mask_t events_to_watch,
    fj_unixpoller_callback_fn_t * callback
)
{
    FJ_INIT_TRY

    struct pollfd pollfd = {
        .fd = file_descriptor,
        .events = events_to_watch,
        .revents = 0,
    };

    fj_try fj_vec_push_item(&poller->pollfds, &pollfd);
    fj_else {
        return fj_result;
    }

    fj_try fj_vec_push_item(&poller->callbacks, &callback);
    fj_else {
        return fj_result;
    }

    return FJ_OK;
}


static
fj_err_t remove_index(struct fj_unixpoller * poller, uint32_t index)
{
    FJ_INIT_TRY

    fj_try fj_vec_remove_items(&poller->pollfds, index, 1);
    fj_else {
        return fj_result;
    }

    fj_try fj_vec_remove_items(&poller->callbacks, index, 1);
    fj_else {
        return fj_result;
    }

    return FJ_OK;
}


fj_err_t fj_unixpoller_remove(struct fj_unixpoller * poller, fj_unixpoller_fd_t file_descriptor)
{
    FJ_INIT_TRY

    struct pollfd * pollfds = poller->pollfds.items;

    for (uint32_t i=0; i<poller->pollfds.length; i++) {
        if (pollfds->fd == file_descriptor) {
            fj_try remove_index(poller, i);
            fj_else {
                return fj_result;
            }

            break;
        }
    }

    return FJ_OK;
}


static
int32_t to_poll_timeout(fj_seconds_t timeout)
{
    if (isinf(timeout)) {
        return -1;
    }

    if (timeout <= 0.0 || isnan(timeout)) {
        return 0;
    }

    // milliseconds
    return (int32_t) (timeout * 1000.0);
}

fj_err_t fj_unixpoller_poll(struct fj_unixpoller * poller)
{
    int32_t result = poll(
        poller->pollfds.items,
        poller->pollfds.length,
        to_poll_timeout(poller->timeout)
    );

    if (result < 0) {
        return FJ_ERR_IO_ERROR;
    }

    if (result == 0) {
        return FJ_OK;
    }

    return process_events(poller);
}


fj_bool32_t fj_unixpoller_should_finish(struct fj_unixpoller * poller)
{
    return isnan(poller->timeout);
}
