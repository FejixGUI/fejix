#ifndef FEJIX_INTERFACE_CLIENT_H_
#define FEJIX_INTERFACE_CLIENT_H_


#include <fejix/core/base.h>


typedef uint32_t fj_client_schedule_type_t;

enum fj_client_schedule_type {
    FJ_CLIENT_SCHEDULE_WAIT,
    FJ_CLIENT_SCHEDULE_QUIT,
};


typedef uint32_t fj_client_run_type_t;

enum fj_client_run_type {
    /** Represents most kinds of main program entrypoints. */
    FJ_CLIENT_RUN_MAIN,
};


struct fj_client FJ_PUBLICLY({ union fj_tag tag; });


struct fj_client_schedule {
    /**
    Used when type is ``FJ_CLIENT_SCHEDULE_WAIT``.
    Accepts infinity for infinite waiting. Default is 0.
    */
    fj_seconds_t timeout;

    /** Default is ``FJ_CLIENT_SCHEDULE_WAIT``. */
    fj_client_schedule_type_t type;
};


struct fj_client_info {
    union fj_tag tag;

    /** Implementation-dependent extra creation information. Set to NULL if unused. */
    void *extra_info;

    /** String that should uniquely identify the app to the shell. */
    char const *name;
};


struct fj_client_callbacks {
    /** Called at the beginning of run() and then after each message processing iteration. */
    fj_err_t (*idle)(struct fj_client *client);
};


struct fj_client_iface {
    /**
    Callbacks and info are deep-copied where applicable.

    :param client: Returns the client or NULL on failure.
    */
    fj_err_t (*create)(
        struct fj_client **client,
        struct fj_client_callbacks const *callbacks,
        struct fj_client_info const *info
    );

    fj_err_t (*destroy)(struct fj_client *client);

    /**
    Runs a message polling loop.
    At the startup and at the end of each polling iteration calls ``idle``.
    */
    fj_err_t (*run)(struct fj_client *client);

    void (*set_schedule)(struct fj_client *client, struct fj_client_schedule const *schedule);

    /** This method is thread-safe provided that the client is not being destroyed. */
    fj_err_t (*wakeup)(struct fj_client *client);
};


#endif
