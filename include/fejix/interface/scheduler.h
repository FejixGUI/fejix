#ifndef FEJIX_INTERFACE_SCHEDULER_H_
#define FEJIX_INTERFACE_SCHEDULER_H_


#include <fejix/interface/client.h>


enum fj_schedule_type {
    FJ_SCHEDULE_SLEEP_FOREVER,
    FJ_SCHEDULE_SLEEP_TIMEOUT,
    FJ_SCHEDULE_CONTINUE,
    FJ_SCHEDULE_QUIT,
};


typedef uint32_t fj_schedule_type_t;


struct fj_scheduler_common;


struct fj_schedule {
    /**
    Used when the schedule type is ``FJ_SCHEDULE_WAIT_TIMEOUT``.
    Default is 0.
    */
    fj_timeout_t timeout;

    /** Default is ``FJ_SCHEDULE_WAIT_FOREVER``. */
    fj_schedule_type_t type;
};

struct fj_scheduler_caps {
    fj_timeout_t timeout_min;
    fj_timeout_t timeout_max;
    fj_timeout_t timeout_resolution;
};


struct fj_scheduler_callbacks {
    fj_err_t (*on_schedule)(struct fj_client *client);
};


struct fj_scheduler_iface {
    /**
    :param scheduler: Returns the scheduler instance or NULL on failure or if unsupported.
    :param caps: Returns the scheduler capabilities.
    */
    fj_err_t (*create_common)(
        struct fj_client *client,
        struct fj_scheduler_common **common,
        struct fj_scheduler_callbacks const *callbacks,
        struct fj_scheduler_caps *caps
    );

    fj_err_t (*destroy_common)(struct fj_client *client, struct fj_scheduler_common *common);

    /** Timeout is rounded down to the nearest multiple of precision. */
    void (*set_schedule)(
        struct fj_client *client,
        struct fj_scheduler_common *common,
        struct fj_schedule const *schedule
    );

    /**
    Waking up must be done only from secondary threads and never on the main thread.

    This method is thread-safe provided that the client is not being destroyed and this interface
    is not being deinitialised.
    */
    fj_err_t (*wakeup)(struct fj_client *client, struct fj_scheduler_common *common);
};


#endif
