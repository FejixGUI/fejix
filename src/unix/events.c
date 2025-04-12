#include <src/unix/events.h>

#include <fejix/utils/math.h>

#include <unistd.h>

#include <math.h>


static fj_err process_events(struct fj_unix_events *events)
{
    for (size_t i = 0; i < events->pollfds.length; i++) {
        if (events->pollfds.items[i].revents == 0) {
            continue;
        }

        FJ_TRY (events->callbacks.items[i](
                    events->callback_data,
                    events->pollfds.items[i].fd,
                    events->pollfds.items[i].revents))
        {
            return fj_result;
        }

        events->pollfds.items[i].revents = 0;
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


fj_err fj_unix_events_init(struct fj_unix_events *events, void *callback_data)
{
    *events = (struct fj_unix_events) {
        .callback_data = callback_data,
    };

    int pipe_result = pipe((int32_t *) events->wakeup_pipe);

    if (pipe_result < 0) {
        return FJ_ERR_IO_FAILED;
    }

    FJ_TRY (fj_unix_events_add(events, events->wakeup_pipe[0], POLLIN, handle_wakeup)) {
        fj_unix_events_deinit(events);
        return fj_result;
    }

    return FJ_OK;
}


void fj_unix_events_deinit(struct fj_unix_events *events)
{
    if (events->wakeup_pipe[0] != 0) {
        close(events->wakeup_pipe[0]);
        close(events->wakeup_pipe[1]);
    }

    FJ_VECTOR_FREE(&events->pollfds);
    FJ_VECTOR_FREE(&events->callbacks);
}


fj_err fj_unix_events_add(
    struct fj_unix_events *events,
    int file_descriptor,
    short events_to_watch,
    fj_unix_events_callback callback)
{
    struct pollfd pollfd = {
        .fd = file_descriptor,
        .events = events_to_watch,
        .revents = 0,
    };

    FJ_TRY (FJ_VECTOR_EXPAND(&events->pollfds)) {
        return fj_result;
    }

    events->pollfds.items[events->pollfds.length - 1] = pollfd;

    FJ_TRY (FJ_VECTOR_EXPAND(&events->callbacks)) {
        return fj_result;
    }

    events->callbacks.items[events->callbacks.length - 1] = callback;

    return FJ_OK;
}


fj_err fj_unix_events_remove(struct fj_unix_events *events, int file_descriptor)
{
    for (size_t i = 0; i < events->pollfds.length; i++) {
        if (events->pollfds.items[i].fd == file_descriptor) {
            FJ_TRY (FJ_VECTOR_SHRINK_AT(&events->pollfds, i)) {
                return fj_result;
            }

            FJ_TRY (FJ_VECTOR_SHRINK_AT(&events->callbacks, i)) {
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

fj_err fj_unix_events_wait(struct fj_unix_events *events, fj_time *opt_timeout)
{
    int32_t result
        = poll(events->pollfds.items, events->pollfds.length, into_poll_timeout(opt_timeout));

    if (result < 0) {
        return FJ_ERR_IO_FAILED;
    }

    if (result == 0) {
        return FJ_OK;
    }

    return process_events(events);
}


fj_err fj_unix_events_wakeup(struct fj_unix_events *events)
{
    uint8_t buffer[1] = { 42 };  // arbitrary number
    ssize_t written_count = write(events->wakeup_pipe[1], buffer, 1);

    if (written_count < 0) {
        return FJ_ERR_IO_FAILED;
    }

    return FJ_OK;
}
