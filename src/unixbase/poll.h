#ifndef FEJIX_UNIXBASE_POLL_H_INCLUDED
#define FEJIX_UNIXBASE_POLL_H_INCLUDED


#include <fejix/core.h>
#include <fejix/utils/vector.h>

#include <sys/poll.h>


typedef fj_err (*fj_unixbase_poll_callback)(
    void *callback_data, int file_descriptor, short event_mask);


FJ_DEFINE_VECTOR(fj_unixbase_poll_fd_vector, struct pollfd)
FJ_DEFINE_VECTOR(fj_unixbase_poll_callback_vector, fj_unixbase_poll_callback)

struct fj_unixbase_poll {
    int wakeup_pipe[2];
    struct fj_unixbase_poll_fd_vector pollfds;
    struct fj_unixbase_poll_callback_vector callbacks;
    void *callback_data;
};


fj_err fj_unixbase_poll_init(struct fj_unixbase_poll *mpoll, void *callback_data);

void fj_unixbase_poll_deinit(struct fj_unixbase_poll *mpoll);

fj_err fj_unixbase_poll_add(
    struct fj_unixbase_poll *mpoll,
    int file_descriptor,
    short events_to_watch,
    fj_unixbase_poll_callback callback);

/**
    Ensures that the file descriptor is not being watched.
    If the file descriptor has not beed added to the watching list, this
    returns `FJ_OK`.
*/
fj_err fj_unixbae_poll_remove(struct fj_unixbase_poll *mpoll, int file_descriptor);

fj_err fj_unixbase_poll_wait(struct fj_unixbase_poll *mpoll, fj_time *opt_timeout);

fj_err fj_unixbase_poll_wakeup(struct fj_unixbase_poll *mpoll);


#endif
