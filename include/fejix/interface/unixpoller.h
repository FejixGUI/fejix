#ifndef FEJIX_UNIXPOLLER_H_
#define FEJIX_UNIXPOLLER_H_


#include <fejix/core/base.h>
#include <fejix/interface/client.h>


/* UNIX file descriptor. */
typedef uint32_t fj_unixpoller_fd_t;

/* Flags returned from select/poll. */
typedef uint16_t fj_unixpoller_event_mask_t;

typedef fj_err_t (*fj_unixpoller_callback_t)(
    struct fj_client * client,
    fj_unixpoller_fd_t file_descriptor,
    fj_unixpoller_event_mask_t received_events
);


struct fj_unixpoller {
    fj_err_t (*watch_fd)(
        struct fj_client * client,
        fj_unixpoller_fd_t file_descriptor,
        fj_unixpoller_event_mask_t events_to_watch,
        fj_unixpoller_callback_t callabck
    );

    fj_err_t (*unwatch_fd)(
        struct fj_client * client,
        fj_unixpoller_fd_t file_descriptor
    );
};

struct fj_unixpoller_listener {
    fj_err_t (*init)(
        struct fj_client * client,
        struct fj_unixpoller * interface
    );
};


#endif
