#ifndef FJ_TIMER_H_INCLUDED
#define FJ_TIMER_H_INCLUDED


#include <fejix/io.h>


enum { FJ_TIMER_PROTOCOL = 1 };


typedef uint32_t fj_timer_type_t;

enum {
    FJ_TIMER_GENERIC,
    FJ_TIMER_PRECISE,
};


enum {
    FJ_TIMER_TIMEOUT,
};

struct fj_timer_timeout {
    uint8_t _unused;
};


struct fj_timer {
    fj_err_t (*create_timer)(
        struct fj_io_connection *conn,
        struct fj_io_element **out_timer,
        fj_timer_type_t timer_type
    );

    fj_err_t (*destroy_timer)(
        struct fj_io_connection *conn,
        struct fj_io_element *timer
    );

    fj_err_t (*set_interval)(
        struct fj_io_connection *conn,
        struct fj_io_element *timer,
        fj_microseconds_t interval
    );

    fj_err_t (*set_periodic)(
        struct fj_io_connection *conn,
        struct fj_io_element *timer,
        bool periodic
    );

    fj_err_t (*get_remaining_time)(
        struct fj_io_connection *conn,
        struct fj_io_element *timer,
        fj_microseconds_t *out_remaining_time
    );
};


#endif
