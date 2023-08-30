#ifndef FEJIX_INTERNAL_UNIXPOLLER_H_
#define FEJIX_INTERNAL_UNIXPOLLER_H_


#include <fejix/interface/unixpoller.h>


struct fj_unixpoller;


struct fj_unixpoller * fj_unixpoller_new(void);

void fj_unixpoller_del(struct fj_unixpoller * poller);

fj_err_t fj_unixpoller_add_watch(
    struct fj_unixpoller * poller,
    fj_unixpoller_fd_t file_descriptor,
    fj_unixpoller_event_mask_t events_to_watch,
    fj_unixpoller_callback_t callback
);

fj_err_t fj_unixpoller_remove_watch(
    struct fj_unixpoller * poller,
    fj_unixpoller_fd_t file_descriptor
);

/* # Parameters
    * `client` - unused by `poll` itself, but passed to the callbacks.
    * `poll_forever` - if `true`, ignores the timeout and polls until any event
        is received. */
fj_err_t fj_unixpoller_poll(
    struct fj_unixpoller * poller,
    struct fj_client * client,
    uint32_t timeout_ms,
    fj_bool_t poll_forever
);

fj_unixpoller_fd_t fj_unixpoller_get_interruptor(
    struct fj_unixpoller * poller
);

/* Used to wakeup `fj_unixpoller_poll` from other threads. */
fj_err_t fj_unixpoller_interrupt(
    fj_unixpoller_fd_t interruptor
);


#endif
