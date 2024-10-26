#ifndef FEJIX_INTERFACE_SLEEP_TIMER_H_
#define FEJIX_INTERFACE_SLEEP_TIMER_H_


#include <fejix/interface/client.h>


struct fj_sleep_timer_manager;


struct fj_sleep_timer_manger_desc {
    fj_timeout_t timeout_min;
    fj_timeout_t timeout_max;
};


struct fj_sleep_timer_funcs {
    /**
    :param manager: Returns a timer manager, NULL on failure or if unsupported.
    :param caps: Returns the timer capabilities.
    */
    fj_err_t (*create_manager)(
        struct fj_sleep_timer_manager **manager,
        struct fj_sleep_timer_manger_desc *caps,
        struct fj_client *client
    );

    fj_err_t (*destroy_manager)(struct fj_sleep_timer_manager *manager);

    void (*set_timeout)(struct fj_sleep_timer_manager *manager, fj_timeout_t timeout);

    void (*unset_timeout)(struct fj_sleep_timer_manager *manager);
};


#endif
