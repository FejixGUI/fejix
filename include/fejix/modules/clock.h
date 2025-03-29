#ifndef FJ_CLOCK_H_INCLUDED
#define FJ_CLOCK_H_INCLUDED


#include <fejix/io.h>


enum { FJ_CLOCK = 1 };


enum {
    FJ_CLOCK_EVENT_TICK = (FJ_CLOCK << 16) + 0,
};


typedef uint32_t fj_clock_type;

enum {
    FJ_CLOCK_TYPE_GENERIC = 0,
    FJ_CLOCK_TYPE_PRECISE = 1,
};


typedef struct fj_clock_protocol {
    fj_err (*init)(fj_io_connection *conn);

    fj_err (*deinit)(fj_io_connection *conn);

    fj_err (*create_clock)(
        fj_io_connection *conn, fj_clock_type clock_type, fj_io_element **out_clock);

    fj_err (*destroy_clock)(fj_io_connection *conn, fj_io_element *clock);

    fj_err (*start_ticking)(fj_io_connection *conn, fj_io_element *clock, fj_time interval);

    fj_err (*stop_ticking)(fj_io_connection *conn, fj_io_element *clock);

    fj_err (*get_remaining_time)(
        fj_io_connection *conn, fj_io_element *clock, fj_time *out_remaining_time);
} fj_clock_protocol;


#endif
