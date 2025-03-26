#ifndef FJ_CLOCK_H_INCLUDED
#define FJ_CLOCK_H_INCLUDED


#include <fejix/io.h>


enum { FJ_CLOCK = 1 };


enum {
    FJ_CLOCK_EVENT_TICK = (FJ_CLOCK << 16) + 0,
};


typedef uint32_t fj_clock_type_t;

enum {
    FJ_CLOCK_TYPE_GENERIC = 0,
    FJ_CLOCK_TYPE_PRECISE = 1,
};


typedef struct fj_clock_protocol {
    fj_err_t (*init)(fj_io_connection_t *conn);

    fj_err_t (*deinit)(fj_io_connection_t *conn);

    fj_err_t (*create_clock)(
        fj_io_connection_t *conn, fj_clock_type_t clock_type, fj_io_element_t **out_clock);

    fj_err_t (*destroy_clock)(fj_io_connection_t *conn, fj_io_element_t *clock);

    fj_err_t (*start_ticking)(fj_io_connection_t *conn, fj_io_element_t *clock, fj_time_t interval);

    fj_err_t (*stop_ticking)(fj_io_connection_t *conn, fj_io_element_t *clock);

    fj_err_t (*get_remaining_time)(
        fj_io_connection_t *conn, fj_io_element_t *clock, fj_time_t *out_remaining_time);
} fj_clock_protocol_t;


#endif
