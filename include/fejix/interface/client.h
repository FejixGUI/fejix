#ifndef FEJIX_CLIENT_H_
#define FEJIX_CLIENT_H_


#include <fejix/implementation.h>


FJ_DEFINE_UNIQUE_TYPE(fj_client_data_t)


enum fj_client_serve_type {
    /** Represents most kinds of main program entrypoints. */
    FJ_CLIENT_SERVE_TYPE_MAIN,
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

/*extendable*/
struct fj_client_callback {
    fj_err_t (* call)(
        struct fj_client_callback const * callback,
        struct fj_message const * message
    );
};

/*extendable*/
struct fj_client_interrupt_signal {
    fj_err_t (* interrupt)(
        struct fj_client_interrupt_signal const * signal
    );
};

struct fj_client_info {
    uint8_t const */*[]*/ name;
    struct fj_client_callback const * callback;
};

struct fj_client {
    fj_err_t (* create)(
        fj_client_data_t */*?*/ /*out*/ * client,
        struct fj_client_info const * info
    );

    fj_err_t (* destroy)(
        fj_client_data_t * client
    );

    fj_err_t (* serve)(
        fj_client_data_t * client,
        fj_enum32_t serve_type,
        void * serve_data
    );

    /** timeout also accepts 0.0 (no wait), INFINITY (wait forever),
        and NAN (quit). */
    void (* set_timeout)(
        fj_client_data_t * client,
        fj_seconds_t timeout
    );

    void (* get_interrupt_signal)(
        fj_client_data_t * client,
        struct fj_client_interrupt_signal const * /*out*/ * signal
    );
};


#endif
