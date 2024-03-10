#ifndef FEJIX_INTERNAL_FDPOLL_H_
#define FEJIX_INTERNAL_FDPOLL_H_


#include <fejix/base.h>
#include <fejix/vec.h>

#include <sys/poll.h>


typedef int32_t fj_fdpoll_fd_t;

typedef int16_t fj_fdpoll_event_mask_t;

typedef fj_err_t (fj_fdpoll_callback_fn_t)(
    void * callback_data,
    fj_fdpoll_fd_t file_descriptor,
    fj_fdpoll_event_mask_t event_mask
);


struct fj_fdpoll {
    fj_fdpoll_fd_t interrupt_pipe[2];
    struct fj_vec pollfds;
    struct fj_vec callbacks;
};


fj_err_t fj_fdpoll_init(struct fj_fdpoll FJ_OUT * poller);

void fj_fdpoll_deinit(struct fj_fdpoll * poller);

fj_err_t fj_fdpoll_add(
    struct fj_fdpoll * poller,
    fj_fdpoll_fd_t file_descriptor,
    fj_fdpoll_event_mask_t events_to_watch,
    fj_fdpoll_callback_fn_t * callback
);

/** Ensures that the file descriptor is not being watched.
    If the file descriptor has not beed added to the watching list, this
    returns `FJ_OK`. */
fj_err_t fj_fdpoll_remove(
    struct fj_fdpoll * poller,
    fj_fdpoll_fd_t file_descriptor
);

/** === ARGS ===
    - `client` - unused by `poll()` itself, but passed to the callbacks.
    - `timeout` - if UINT32_MAX, this will poll forever, if zero, this will
        return immediately. */
fj_err_t fj_fdpoll_poll(
    struct fj_fdpoll * poller,
    uint32_t timeout_ms,
    void * callback_data
);

fj_fdpoll_fd_t fj_fdpoll_get_interruptor(
    struct fj_fdpoll * poller
);

/** Used to wakeup `fj_fdpoll_poll` from other threads. */
fj_err_t fj_fdpoll_interrupt(
    fj_fdpoll_fd_t interruptor
);


#endif
