#include <src/unix/events.h>

#include <fejix/utils/math.h>

#include <unistd.h>

#include <math.h>


static enum fj_error process_events(struct fj_unix_events *events)
{
    enum fj_error e;

    for (size_t i = 0; i < events->pollfds.length; i++) {
        if (events->pollfds.items[i].revents == 0) {
            continue;
        }

        e = events->callbacks.items[i](
            events->callback_data, events->pollfds.items[i].fd, events->pollfds.items[i].revents);

        if (e)
            return e;

        events->pollfds.items[i].revents = 0;
    }

    return FJ_OK;
}


static enum fj_error handle_wakeup(void *callback_data, int fd, short events)
{
    (void) callback_data;

    if (events & (POLLERR | POLLHUP | POLLNVAL)) {
        return FJ_ERROR_IO_FAILED;
    }

    uint8_t buffer[1];
    read(fd, buffer, 1);

    return FJ_OK;
}


enum fj_error fj_unix_events_init(struct fj_unix_events *events, void *callback_data)
{
    enum fj_error e;

    *events = (struct fj_unix_events) {
        .callback_data = callback_data,
    };

    int pipe_result = pipe((int32_t *) events->wakeup_pipe);

    if (pipe_result < 0) {
        return FJ_ERROR_IO_FAILED;
    }

    e = fj_unix_events_add(events, events->wakeup_pipe[0], POLLIN, handle_wakeup);

    if (e) {
        fj_unix_events_deinit(events);
        return e;
    }

    return FJ_OK;
}


void fj_unix_events_deinit(struct fj_unix_events *events)
{
    if (events->wakeup_pipe[0] != 0) {
        close(events->wakeup_pipe[0]);
        close(events->wakeup_pipe[1]);
    }

    fj_unix_events_pollfd_vector_free(&events->pollfds);
    fj_unix_events_callback_vector_free(&events->callbacks);
}


enum fj_error fj_unix_events_add(
    struct fj_unix_events *events,
    int file_descriptor,
    short events_to_watch,
    fj_unix_events_callback callback)
{
    enum fj_error e;

    struct pollfd pollfd = {
        .fd = file_descriptor,
        .events = events_to_watch,
        .revents = 0,
    };

    e = fj_unix_events_pollfd_vector_push(&events->pollfds, &pollfd);

    if (e)
        return e;

    e = fj_unix_events_callback_vector_push(&events->callbacks, &callback);

    if (e)
        return e;

    return FJ_OK;
}


enum fj_error fj_unix_events_remove(struct fj_unix_events *events, int file_descriptor)
{
    enum fj_error e;

    for (size_t i = 0; i < events->pollfds.length; i++) {
        if (events->pollfds.items[i].fd == file_descriptor) {
            e = fj_unix_events_pollfd_vector_remove(&events->pollfds, i);
            if (e)
                return e;

            e = fj_unix_events_callback_vector_remove(&events->callbacks, i);
            if (e)
                return e;

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

enum fj_error fj_unix_events_wait(struct fj_unix_events *events, fj_time *opt_timeout)
{
    int32_t result
        = poll(events->pollfds.items, events->pollfds.length, into_poll_timeout(opt_timeout));

    if (result < 0) {
        return FJ_ERROR_IO_FAILED;
    }

    if (result == 0) {
        return FJ_OK;
    }

    return process_events(events);
}


enum fj_error fj_unix_events_wakeup(struct fj_unix_events *events)
{
    uint8_t buffer[1] = { 42 };  // arbitrary number
    ssize_t written_count = write(events->wakeup_pipe[1], buffer, 1);

    if (written_count < 0) {
        return FJ_ERROR_IO_FAILED;
    }

    return FJ_OK;
}
