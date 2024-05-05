#ifndef FEJIX_PLATFORM_IPC_UNIXPOLL_H_
#define FEJIX_PLATFORM_IPC_UNIXPOLL_H_


#include <fejix/base.h>
#include <fejix/vec.h>

#include <sys/poll.h>


typedef int32_t fj_unixfd_t;

typedef int16_t fj_unixpoll_event_mask_t;

typedef fj_err_t (fj_unixpoll_callback_fn_t)(
    void * callback_data,
    fj_unixfd_t file_descriptor,
    fj_unixpoll_event_mask_t event_mask
);


struct fj_unixpoller {
    fj_unixfd_t interrupt_pipe[2];
    struct fj_vec pollfds;
    struct fj_vec callbacks;
};


fj_err_t fj_unixpoll_init(struct fj_unixpoller fjOUT * poller);

void fj_unixpoll_deinit(struct fj_unixpoller * poller);

fj_err_t fj_unixpoll_add(
    struct fj_unixpoller * poller,
    fj_unixfd_t file_descriptor,
    fj_unixpoll_event_mask_t events_to_watch,
    fj_unixpoll_callback_fn_t * callback
);

/** Ensures that the file descriptor is not being watched.
    If the file descriptor has not beed added to the watching list, this
    returns `FJ_OK`. */
fj_err_t fj_unixpoll_remove(
    struct fj_unixpoller * poller,
    fj_unixfd_t file_descriptor
);

/** timeout - if UINT32_MAX, this will poll forever, if zero, this will
        return immediately. */
fj_err_t fj_unixpoll_poll(
    struct fj_unixpoller * poller,
    uint32_t timeout_ms,
    void * callback_data
);

fj_unixfd_t fj_unixpoll_get_interruptor(
    struct fj_unixpoller * poller
);

/** Used to wakeup `fj_unixpoll_poll` from other threads. */
fj_err_t fj_unixpoll_interrupt(
    fj_unixfd_t interruptor
);


#endif
