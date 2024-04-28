#ifndef FEJIX_SERVER_H_
#define FEJIX_SERVER_H_


#include <fejix/implementation.h>
#include <fejix/interface/instance.h>


FJ_DEFINE_HANDLE(fj_server_t)


enum fj_server_serve_type {
    /** Represents most kinds of main program entrypoints. */
    FJ_SERVER_SERVE_TYPE_MAIN,
};

enum fj_server_schedule_type {
    FJ_SERVER_SCHEDULE_TYPE_CONTINUE,
    FJ_SERVER_SCHEDULE_TYPE_WAIT_TIMEOUT,
    FJ_SERVER_SCHEDULE_TYPE_WAIT_FOREVER,
    FJ_SERVER_SCHEDULE_TYPE_QUIT,
};

enum fj_server_message_id {
    FJ_SERVER_CONTINUE,
};


struct fj_message {
    fj_enum32_t sender_interface_id;
    fj_enum32_t message_id;
    void * sender_object;
    void *fjOPTION message_data;
};

typedef fj_err_t (fj_server_callback_fn_t)(
    void * callback_data,
    fj_server_t * server,
    struct fj_message const * message
);

struct fj_server_info {
    void * callback_data;
    fj_server_callback_fn_t * callback;
};

fjINHERITABLE
struct fj_server_signal {
    fj_err_t (* signal)(
        struct fj_server_signal const * signal
    );
};

struct fj_server_schedule {
    fj_enum32_t schedule_type;

    fj_nanoseconds_t timeout;
};

struct fj_server {
    fj_err_t (* create)(
        fj_instance_t * instance,
        struct fj_server_info const * info,
        fj_server_t * fjOUT * server
    );

    fj_err_t (* destroy)(
        fj_server_t * server
    );

    fj_err_t (* serve)(
        fj_server_t * server,
        fj_enum32_t serve_type,
        void * serve_data
    );

    void (* get_signal)(
        fj_server_t * server,
        struct fj_server_signal const * fjOUT * signal
    );

    void (* set_schedule)(
        fj_server_t * server,
        struct fj_server_schedule const * schedule
    );
};


#endif
