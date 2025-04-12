#include <src/unixbase/poll.h>

#include <fejix/utils/math.h>

#include <unistd.h>

#include <math.h>


static fj_err process_events(struct fj_unixbase_poll *mpoll)
{
    struct pollfd *pollfds = mpoll->pollfds.items;
    fj_unixbase_poll_callback *callbacks = mpoll->callbacks.items;

    for (size_t i = 0; i < mpoll->pollfds.length; i++) {
        if (pollfds[i].revents == 0) {
            continue;
        }

        FJ_TRY (callbacks[i](mpoll->callback_data, pollfds[i].fd, pollfds[i].revents)) {
            return fj_result;
        }

        pollfds[i].revents = 0;
    }

    return FJ_OK;
}


static fj_err handle_wakeup(void *callback_data, int fd, short events)
{
    (void) callback_data;

    if (events & (POLLERR | POLLHUP | POLLNVAL)) {
        return FJ_ERR_IO_FAILED;
    }

    uint8_t buffer[1];
    read(fd, buffer, 1);

    return FJ_OK;
}


fj_err fj_unixbase_poll_init(struct fj_unixbase_poll *mpoll, void *callback_data)
{
    *mpoll = (struct fj_unixbase_poll) {
        .callback_data = callback_data,
    };

    int pipe_result = pipe((int32_t *) mpoll->wakeup_pipe);

    if (pipe_result < 0) {
        return FJ_ERR_IO_FAILED;
    }

    FJ_TRY (fj_unixbase_poll_add(mpoll, mpoll->wakeup_pipe[0], POLLIN, handle_wakeup)) {
        fj_unixbase_poll_deinit(mpoll);
        return fj_result;
    }

    return FJ_OK;
}


void fj_unixbase_poll_deinit(struct fj_unixbase_poll *mpoll)
{
    if (mpoll->wakeup_pipe[0] != 0) {
        close(mpoll->wakeup_pipe[0]);
        close(mpoll->wakeup_pipe[1]);
    }

    FJ_VECTOR_FREE(&mpoll->pollfds);
    FJ_VECTOR_FREE(&mpoll->callbacks);
}


fj_err fj_unixbase_poll_add(
    struct fj_unixbase_poll *mpoll,
    int file_descriptor,
    short events_to_watch,
    fj_unixbase_poll_callback callback)
{
    struct pollfd pollfd = {
        .fd = file_descriptor,
        .events = events_to_watch,
        .revents = 0,
    };

    FJ_TRY (FJ_VECTOR_EXPAND(&mpoll->pollfds)) {
        return fj_result;
    }

    mpoll->pollfds.items[mpoll->pollfds.length - 1] = pollfd;

    FJ_TRY (FJ_VECTOR_EXPAND(&mpoll->callbacks)) {
        return fj_result;
    }

    mpoll->callbacks.items[mpoll->callbacks.length - 1] = callback;

    return FJ_OK;
}


static fj_err remove_index(struct fj_unixbase_poll *mpoll, uint32_t index)
{
    FJ_TRY (FJ_VECTOR_SHRINK_AT(&mpoll->pollfds, index)) {
        return fj_result;
    }

    FJ_TRY (FJ_VECTOR_SHRINK_AT(&mpoll->callbacks, index)) {
        return fj_result;
    }

    return FJ_OK;
}


fj_err fj_unixbae_poll_remove(struct fj_unixbase_poll *mpoll, int file_descriptor)
{
    struct pollfd *pollfds = mpoll->pollfds.items;

    for (size_t i = 0; i < mpoll->pollfds.length; i++) {
        if (pollfds->fd == file_descriptor) {
            FJ_TRY (remove_index(mpoll, i)) {
                return fj_result;
            }

            break;
        }
    }

    return FJ_OK;
}


static int32_t into_poll_timeout(fj_time *opt_timeout)
{
    if (opt_timeout == NULL) {
        return -1;
    }

    return (int32_t) fj_time_into_millis(*opt_timeout);
}

fj_err fj_unixbase_poll_wait(struct fj_unixbase_poll *mpoll, fj_time *opt_timeout)
{
    int32_t result
        = poll(mpoll->pollfds.items, mpoll->pollfds.length, into_poll_timeout(opt_timeout));

    if (result < 0) {
        return FJ_ERR_IO_FAILED;
    }

    if (result == 0) {
        return FJ_OK;
    }

    return process_events(mpoll);
}


fj_err fj_unixbase_poll_wakeup(struct fj_unixbase_poll *mpoll)
{
    uint8_t buffer[1] = { 42 };
    ssize_t written_count = write(mpoll->wakeup_pipe[1], buffer, 1);

    if (written_count < 0) {
        return FJ_ERR_IO_FAILED;
    }

    return FJ_OK;
}
