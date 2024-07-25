#ifndef FEJIX_UNIXPOLLER_H_
#define FEJIX_UNIXPOLLER_H_


#include <fejix/interface/client.h>

#include <fejix/core/base.h>
#include <fejix/core/vec.h>

#include <sys/poll.h>


typedef int32_t fj_unixpoller_fd_t;

typedef int16_t fj_unixpoller_event_mask_t;

typedef fj_err_t (fj_unixpoller_callback_fn_t)(
    void * callback_data,
    fj_unixpoller_fd_t file_descriptor,
    fj_unixpoller_event_mask_t event_mask
);


struct fj_unixpoller_waker {
    struct fj_client_waker base;
    fj_unixpoller_fd_t wakeup_fd;
};

struct fj_unixpoller {
    struct fj_unixpoller_waker waker;
    fj_unixpoller_fd_t waker_pipe[2];

    fj_seconds_t timeout;
    
    struct fj_vec pollfds;
    struct fj_vec callbacks;
    void * callback_data;
};


fj_err_t fj_unixpoller_init(
    struct fj_unixpoller /*out*/ * this,
    void * callback_data
);

void fj_unixpoller_deinit(
    struct fj_unixpoller * this
);

fj_err_t fj_unixpoller_add(
    struct fj_unixpoller * this,
    fj_unixpoller_fd_t file_descriptor,
    fj_unixpoller_event_mask_t events_to_watch,
    fj_unixpoller_callback_fn_t * callback
);

/** Ensures that the file descriptor is not being watched.
    If the file descriptor has not beed added to the watching list, this
    returns `FJ_OK`. */
fj_err_t fj_unixpoller_remove(
    struct fj_unixpoller * this,
    fj_unixpoller_fd_t file_descriptor
);

fj_err_t fj_unixpoller_poll(
    struct fj_unixpoller * this
);

fj_bool32_t fj_unixpoller_should_finish(
    struct fj_unixpoller * this
);


#endif
