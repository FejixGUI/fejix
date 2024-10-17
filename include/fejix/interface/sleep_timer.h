#ifndef FEJIX_INTERFACE_SLEEP_TIMER_H_
#define FEJIX_INTERFACE_SLEEP_TIMER_H_


#include <fejix/interface/client.h>


struct fj_sleep_timer_impl;


struct fj_sleep_timer_caps {
    fj_timeout_t timeout_min;
    fj_timeout_t timeout_max;
};


struct fj_sleep_timer_iface {
    /**
    :param impl: Returns the timer implementation or NULL on failure or if unsupported.
    :param caps: Returns the timer capabilities.
    */
    fj_err_t (*create_impl)(
        struct fj_client *client,
        struct fj_sleep_timer_impl **impl,
        struct fj_sleep_timer_caps *caps
    );

    fj_err_t (*destroy_impl)(struct fj_client *client, struct fj_sleep_timer_impl *impl);

    void (*set_timeout)(
        struct fj_client *client,
        struct fj_sleep_timer_impl *impl,
        fj_timeout_t timeout
    );

    void (*unset_timeout)(struct fj_client *client, struct fj_sleep_timer_impl *impl);
};


#endif
