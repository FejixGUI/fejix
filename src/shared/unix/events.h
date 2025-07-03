#ifndef FEJIX_UNIX_EVENTS_H_
#define FEJIX_UNIX_EVENTS_H_


#include <src/shared/common/list.h>

#include <fejix/base.h>

#include <sys/poll.h>


/**
    The callback needs to handle errors on its own.
    If gets error events and the errors are fatal, it needs to also return an error in order for
    fj_unix_events_wait to return an error.
*/
typedef fj_err (*fj_unix_events_callback)(
    void *callback_data, int file_descriptor, short event_mask);


FJ_LIST(fj_unix_events_pollfd_list, struct pollfd)
FJ_LIST(fj_unix_events_callback_list, fj_unix_events_callback)

struct fj_unix_events
{
    int ping_pipe[2];
    struct fj_unix_events_pollfd_list pollfds;
    struct fj_unix_events_callback_list callbacks;
    void *callback_data;
};


fj_err fj_unix_events_init(struct fj_unix_events *events, void *callback_data);

void fj_unix_events_deinit(struct fj_unix_events *events);

fj_err fj_unix_events_add(
    struct fj_unix_events *events,
    int file_descriptor,
    short events_to_watch,
    fj_unix_events_callback callback);

/**
    Ensures that the file descriptor is not being watched.
    If the file descriptor has not beed added to the watching list, this
    returns `FJ_OK`.
*/
fj_err fj_unix_events_remove(struct fj_unix_events *events, int file_descriptor);

fj_err fj_unix_events_wait(struct fj_unix_events *events, fj_time *opt_timeout);

fj_err fj_unix_events_ping(struct fj_unix_events *events);

// TODO timers (timerfd on Linux&FreeBSD, timer queue for others)


#endif
