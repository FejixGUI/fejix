#ifndef FEJIX_INTERFACE_CLIENT_H_
#define FEJIX_INTERFACE_CLIENT_H_


#include <fejix/core/base.h>


typedef uint32_t fj_client_run_type_t;

enum fj_client_run_type {
    /** Represents most kinds of main program entrypoints. */
    FJ_CLIENT_RUN_MAIN,
};


struct fj_client {
    union fj_tag tag;
};


struct fj_client_info {
    union fj_tag tag;

    /** String that should uniquely identify the app to the shell. */
    char const *name;
};


struct fj_client_callbacks {
    /** Called at the beginning of run() and then after each message processing iteration. */
    fj_err_t (*idle)(struct fj_client *client);
};


struct fj_client_iface {
    /** Callbacks and info are deep-copied. */
    fj_err_t (*create)(
        struct fj_client * /*? out*/ *client,
        struct fj_client_callbacks const *callbacks,
        struct fj_client_info const *info
    );

    fj_err_t (*destroy)(struct fj_client *client);

    /** :param run_data: Can be NULL if unused. */
    fj_err_t (*run)(struct fj_client *client, fj_client_run_type_t run_type, void *run_data);

    /**
    Possible timeout values:
    * 0.0 - read a message if available
    * >0.0 - wait for a message for the specified timeout
    * INF - wait for a message forever
    * NAN - exit as soon as possible (some messages may be processed before that)
    * other - undefined behavior

    If the timeout expires and no messages are available, the idle callback is called.
    */
    void (*set_sleep_timeout)(struct fj_client *client, fj_seconds_t timeout);

    /** This method is thread-safe provided that the client is not being destroyed. */
    fj_err_t (*wakeup)(struct fj_client *client);
};


#endif
