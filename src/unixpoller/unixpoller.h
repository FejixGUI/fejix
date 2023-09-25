#ifndef FEJIX_INTERNAL_UNIXPOLLER_H_
#define FEJIX_INTERNAL_UNIXPOLLER_H_


#include <fejix/client.h>

#include <sys/poll.h>


/* Unix file descriptor */
typedef uint32_t fj_unixpoller_fd_t;

/* Example: `POLLIN|POLLOUT` */
typedef uint16_t fj_unixpoller_event_mask_t;

typedef fj_err_t (*fj_unixpoller_callback_t)(
    struct fj_client * client,
    fj_unixpoller_fd_t file_descriptor,
    fj_unixpoller_event_mask_t event_mask
);


struct fj_unixpoller;


struct fj_unixpoller * fj_unixpoller_new(void);

void fj_unixpoller_del(struct fj_unixpoller * poller);

fj_err_t fj_unixpoller_watch(
    struct fj_unixpoller * poller,
    fj_unixpoller_fd_t file_descriptor,
    fj_unixpoller_event_mask_t events_to_watch,
    fj_unixpoller_callback_t callback
);

/* Ensures that the file descriptor is not being watched.
    If the file descriptor has not beed added to the watching list, this
    returns `FJ_OK`. */
fj_err_t fj_unixpoller_unwatch(
    struct fj_unixpoller * poller,
    fj_unixpoller_fd_t file_descriptor
);

/* # Parameters
    * `client` - unused by `poll` itself, but passed to the callbacks.
    * `timeout` - if negative, this will poll forever,
        if zero, this will return immediately. */
fj_err_t fj_unixpoller_poll(
    struct fj_unixpoller * poller,
    struct fj_client * client,
    int32_t timeout_ms
);

fj_unixpoller_fd_t fj_unixpoller_get_interruptor(
    struct fj_unixpoller * poller
);

/* Used to wakeup `fj_unixpoller_poll` from other threads. */
fj_err_t fj_unixpoller_interrupt(
    fj_unixpoller_fd_t interruptor
);


#endif
