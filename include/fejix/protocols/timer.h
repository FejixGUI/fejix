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
    FJ_TIMER_EVENT_TIMER_TIMEOUT,
};


typedef struct fj_timer {
    fj_err_t (*create_timer)(
        fj_io_connection_t *conn, fj_io_element_t **out_timer, fj_timer_type_t timer_type);

    fj_err_t (*destroy_timer)(fj_io_connection_t *conn, fj_io_element_t *timer);

    fj_err_t (*set_interval)(fj_io_connection_t *conn, fj_io_element_t *timer, fj_time_t interval);

    fj_err_t (*set_periodic)(fj_io_connection_t *conn, fj_io_element_t *timer, bool periodic);

    fj_err_t (*get_remaining_time)(
        fj_io_connection_t *conn, fj_io_element_t *timer, fj_time_t *out_remaining_time);
} fj_timer_t;


#endif
