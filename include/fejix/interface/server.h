#ifndef FEJIX_SOCKET_SCHEDULER_H_
#define FEJIX_SOCKET_SCHEDULER_H_


#include <fejix/implementation.h>


enum fj_serve_type {
    /** Represents most kinds of main program entrypoints. */
    FJ_SERVE_TYPE_MAIN,
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


typedef fj_err_t (fj_server_callback_fn_t)(
    void * callback_data,
    void * server_context,
    struct fj_message const * message
);


struct fj_server_info {
    void * callback_data;
    fj_server_callback_fn_t * callback;
};

struct fj_server_signal {
    void * signal_data;

    fj_err_t (* signal)(
        struct fj_server_signal const * signal
    );
};

struct fj_server_schedule {
    fj_enum32_t schedule;

    fj_nanoseconds_t timeout;
};

struct fj_server {
    fj_err_t (* create)(
        void * instance,
        struct fj_server_info const * info,
        void * fjOUT * server_context
    );

    fj_err_t (* destroy)(
        void * server_context
    );

    fj_err_t (* serve)(
        void * server_context,
        fj_enum32_t serve_type,
        void * serve_data
    );

    void (* get_signal)(
        void * server_context,
        struct fj_server_signal fjOUT * signal
    );

    void (* set_schedule)(
        void * server_context,
        struct fj_server_schedule const * schedule
    );
};


#endif
