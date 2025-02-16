#ifndef FEJIX_APP_CLOCK_INCLUDED
#define FEJIX_APP_CLOCK_INCLUDED


#include <fejix/interface/app.h>


typedef uint32_t fj_app_clock_resolution_t;

enum {
    FJ_APP_CLOCK_RESOLUTION_LOW,
    FJ_APP_CLOCK_RESOLUTION_HIGH,
};


struct fj_app_clock;


FJ_PUBLIC
void fj_has_app_clock(void);

FJ_PUBLIC
fj_err_t fj_app_clock_create(
    struct fj_app *app, struct fj_app_clock **out_clock, void *clock_userdata);

FJ_PUBLIC
fj_err_t fj_app_clock_destroy(struct fj_app *app, struct fj_app_clock *clock);


/**
    TODO: docs about high- and low-resolution clocks.

    Timeouts in practice are very imprecise. They have system-defined limits (typically
    ``1..2^31`` milliseconds), may get errors due to system task scheduling, can be rounded to
    system clock granularity (e.g. 1/64 of a second on Windows by default) with random
    unpredictable rounding errors, or even completely drift (the bigger the timeout the bigger
    the drift).

    Timeouts may also be disrespected because the system (e.g. on mobile platforms) can bring
    the program to a deactivated state (temporary loss of focus, e.g. when interrupted by
    another program) or to the background (complete loss of focus, e.g. when exited to the
    "Home" screen). In such cases the client may not be able to wake up until the system brings
    the program to its normal state.
*/
FJ_PUBLIC
fj_err_t fj_app_clock_start_ticking(
    struct fj_app *app,
    struct fj_app_clock *clock,
    fj_microseconds_t tick_interval,
    fj_app_clock_resolution_t flags);

FJ_PUBLIC
fj_err_t fj_app_clock_stop_ticking(struct fj_app *app, struct fj_app_clock *clock);

typedef fj_err_t (*fj_app_clock_on_tick_fn_t)(void *clock_userdata);

FJ_PUBLIC
void fj_app_clock_on_tick(struct fj_app *app, fj_app_clock_on_tick_fn_t callback);

/** Returns 0 if not ticking. */
FJ_PUBLIC
fj_microseconds_t fj_app_clock_get_remaining_time(struct fj_app *app, struct fj_app_clock *clock);


#endif
