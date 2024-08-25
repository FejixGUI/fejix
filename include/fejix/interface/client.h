#ifndef FEJIX_CLIENT_H_
#define FEJIX_CLIENT_H_


#include <fejix/core/base.h>


FJ_DEFINE_OPAQUE_TYPE(fj_client_t)


typedef uint32_t fj_client_run_type_t;

enum fj_client_run_type {
    /** Represents most kinds of main program entrypoints. */
    FJ_CLIENT_RUN_MAIN,
};


struct fj_client_info {
    char const */*[]*/ name;
};


struct fj_client_callbacks {
    /** Called at the beginning of run() and then after each message processing iteration. */
    fj_err_t (* idle)(void */*?*/ data);
};

struct fj_client_iface {
    /** Callbacks are referenced the entire object lifetime.
        Info data is deep-copied. */
    fj_err_t (* create)(
        fj_client_t */*? out*/ * client,
        struct fj_client_callbacks const * callbacks,
        void */*?*/ callback_data,
        struct fj_client_info const * info
    );

    fj_err_t (* destroy)(fj_client_t * client);

    fj_err_t (* run)(fj_client_t * client, fj_client_run_type_t run_type, void */*?*/ run_data);

    /** Initialises all interfaces, whose initialisation was requested.
        Some interfaces may get initialised right inside their initialisation methods
        and therefore call their initialisation callbacks before this method,
        however this method must to be called anyway. */
    fj_err_t (* commit)(fj_client_t * client);

    /** Possible timeout values:
        0.0 - read a message if available
        >0.0 - wait for a message for the specified timeout
        INF - wait for a message forever
        NAN - exit as soon as possible (some messages may be processed before that)
        other values - undefined behavior

        If the timeout expires and no messages are available, the idle callback is called. */
    void (* set_sleep_timeout)(fj_client_t * client, fj_seconds_t timeout);

    /** This method is thread-safe provided that the client is not being destroyed. */
    fj_err_t (* wakeup)(fj_client_t * client);
};


#endif
