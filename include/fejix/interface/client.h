#ifndef FEJIX_CLIENT_H_
#define FEJIX_CLIENT_H_


#include <fejix/implementation.h>


typedef struct fj_client_t fj_client_t;


enum fj_client_run_type {
    /** Represents most kinds of main program entrypoints. */
    FJ_CLIENT_RUN_TYPE_MAIN,
};

enum fj_client_message_id {
    FJ_CLIENT_CONTINUE,
};


struct fj_message {
    fj_enum32_t sender_interface_id;
    fj_enum32_t message_id;
    void * sender_object;
    void */*?*/ message_data;
};

struct fj_client_callback {
    fj_err_t (* call)(
        struct fj_client_callback const * this,
        struct fj_message const * message
    );
};

struct fj_client_waker {
    fj_err_t (* wakeup)(
        struct fj_client_waker const * this
    );
};

struct fj_client_info {
    uint8_t const */*[]*/ name;
    struct fj_client_callback const * callback;
};

struct fj_client {
    fj_err_t (* create)(
        fj_client_t */*? out*/ * this,
        struct fj_client_info const * info
    );

    fj_err_t (* destroy)(
        fj_client_t * this
    );

    fj_err_t (* run)(
        fj_client_t * this,
        fj_enum32_t run_type,
        void * run_data
    );

    void (* set_timeout)(
        fj_client_t * this,
        fj_seconds_t timeout
    );

    struct fj_client_waker const * (* get_waker)(
        fj_client_t * this
    );
};


#endif
