#ifndef FEJIX_APP_TIMER_H_
#define FEJIX_APP_TIMER_H_


#include <fejix/interface/app.h>


typedef uint32_t fj_app_timer_flags_t;

enum fj_app_timer_flags {
    FJ_APP_TIMER_PERIODIC = 1 << 0,

    FJ_APP_TIMER_HIGH_RESOLUTION = 1 << 1,
};


struct fj_app_timer;


FJ_PUBLIC
fj_version_t fj_app_timer_get_interface_version(void);

/**
    TODO: docs about high- and low-precision timers.

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
fj_err_t fj_app_timer_create(
    struct fj_app *app, struct fj_app_timer **out_timer, void *timer_userdata);

FJ_PUBLIC
fj_err_t fj_app_timer_destroy(struct fj_app *app, struct fj_app_timer *timer);


typedef fj_err_t (*fj_app_timer_on_timeout_fn_t)(void *timer_userdata);

FJ_PUBLIC
void fj_app_timer_on_timeout(struct fj_app *app, fj_app_timer_on_timeout_fn_t callback);


FJ_PUBLIC
fj_err_t fj_app_timer_start(
    struct fj_app *app,
    struct fj_app_timer *timer,
    fj_nanoseconds_t timeout,
    fj_app_timer_flags_t flags);

FJ_PUBLIC
fj_err_t fj_app_timer_stop(struct fj_app *app, struct fj_app_timer *timer);


#endif
