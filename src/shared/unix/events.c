#include "events.h"

#include <src/shared/utils/logging.h>
#include <src/shared/utils/math.h>

#include <math.h>

#include <unistd.h>


static enum fj_status process_events(struct fj_unix_events *events)
{
    enum fj_status s;

    for (size_t i = 0; i < events->pollfds.length; i++) {
        if (events->pollfds.items[i].revents == 0) {
            continue;
        }

        s = events->callbacks.items[i](
            events->callback_data, events->pollfds.items[i].fd, events->pollfds.items[i].revents);

        if (s)
            return s;

        events->pollfds.items[i].revents = 0;
    }

    return FJ_OK;
}


static enum fj_status handle_ping(void *callback_data, int fd, short events)
{
    (void) callback_data;

    if (events & (POLLERR | POLLHUP | POLLNVAL)) {
        return FJ_IO_FAILED;
    }

    uint8_t buffer[1];
    read(fd, buffer, 1);

    return FJ_OK;
}


enum fj_status fj_unix_events_init(struct fj_unix_events *events, void *callback_data)
{
    enum fj_status s;

    *events = (struct fj_unix_events) {
        .callback_data = callback_data,
    };

    int pipe_result = pipe((int32_t *) events->ping_pipe);

    if (pipe_result < 0) {
        FJ_ERROR("pipe(2) failed");
        return FJ_IO_FAILED;
    }

    s = fj_unix_events_add(events, events->ping_pipe[0], POLLIN, handle_ping);

    if (s) {
        fj_unix_events_deinit(events);
        return s;
    }

    return FJ_OK;
}


void fj_unix_events_deinit(struct fj_unix_events *events)
{
    if (events->ping_pipe[0] != 0) {
        close(events->ping_pipe[0]);
        close(events->ping_pipe[1]);
    }

    fj_unix_events_pollfd_list_clear(&events->pollfds);
    fj_unix_events_callback_list_clear(&events->callbacks);
}


enum fj_status fj_unix_events_add(
    struct fj_unix_events *events,
    int file_descriptor,
    short events_to_watch,
    fj_unix_events_callback callback)
{
    enum fj_status s;

    struct pollfd pollfd = {
        .fd = file_descriptor,
        .events = events_to_watch,
        .revents = 0,
    };

    s = fj_unix_events_pollfd_list_append(&events->pollfds, &pollfd);

    if (s)
        return s;

    s = fj_unix_events_callback_list_append(&events->callbacks, &callback);

    if (s)
        return s;

    return FJ_OK;
}


enum fj_status fj_unix_events_remove(struct fj_unix_events *events, int file_descriptor)
{
    enum fj_status s;

    for (size_t i = 0; i < events->pollfds.length; i++) {
        if (events->pollfds.items[i].fd == file_descriptor) {
            s = fj_unix_events_pollfd_list_remove(&events->pollfds, i);
            if (s)
                return s;

            s = fj_unix_events_callback_list_remove(&events->callbacks, i);
            if (s)
                return s;

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

enum fj_status fj_unix_events_wait(struct fj_unix_events *events, fj_time *opt_timeout)
{
    int32_t result
        = poll(events->pollfds.items, events->pollfds.length, into_poll_timeout(opt_timeout));

    if (result < 0) {
        FJ_ERROR("poll(2) failed");
        return FJ_IO_FAILED;
    }

    if (result == 0) {
        return FJ_OK;
    }

    return process_events(events);
}


enum fj_status fj_unix_events_ping(struct fj_unix_events *events)
{
    uint8_t buffer[1] = { 42 };  // arbitrary number, only needs to be read in handle_ping
    ssize_t written_count = write(events->ping_pipe[1], buffer, 1);

    if (written_count < 0) {
        return FJ_IO_FAILED;
    }

    return FJ_OK;
}
