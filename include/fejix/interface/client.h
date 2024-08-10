#ifndef FEJIX_CLIENT_H_
#define FEJIX_CLIENT_H_


#include <fejix/implementation.h>


FJ_DEFINE_OPAQUE_TYPE(fj_client_t)


typedef uint32_t fj_client_run_type_t;

enum fj_client_run_type {
    /** Represents most kinds of main program entrypoints. */
    FJ_CLIENT_RUN_TYPE_MAIN,
};


struct fj_client_info {
    uint8_t const */*[]*/ name;
};


struct fj_client_callbacks {
    fj_err_t (* idle)(void * data);
};

struct fj_client {
    /** Callbacks are referenced the entire object lifetime.
        Info data is deep-copied. */
    fj_err_t (* create)(
        fj_client_t */*? out*/ * this,
        struct fj_client_callbacks const * callbacks,
        void * callback_data,
        struct fj_client_info const * info
    );

    fj_err_t (* destroy)(fj_client_t * this);

    fj_err_t (* run)(fj_client_t * this, fj_client_run_type_t run_type, void * run_data);

    /** timeout:
        0.0 - do not wait, just read what events are available
        positive value - wait until either the timeout expires or new events come
        INF - wait forever until new events come
        NAN - exit */
    void (* set_timeout)(fj_client_t * this, fj_seconds_t timeout);

    /** This method is thread-safe. */
    fj_err_t (* wakeup)(fj_client_t * this);
};


#endif
