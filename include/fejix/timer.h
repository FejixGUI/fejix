#ifndef FEJIX_TIMER_H_
#define FEJIX_TIMER_H_


#include <fejix/app.h>


enum fj_timer_precision {
    FJ_TIMER_PRECISION_HIGH,
    FJ_TIMER_PRECISION_NORMAL,
    FJ_TIMER_PRECISION_LOW,

    FJ_TIMER_PRECISION_ENUM32 = INT32_MAX,
};


FJ_HANDLE(fj_timer)


FJ_API(fj_timer_init, struct fj_app *app)

FJ_API(
    fj_timer_new,
    struct fj_app *app,
    struct fj_timer **out_timer,
    enum fj_timer_precision precision)

FJ_API(fj_timer_del, struct fj_timer *timer)

FJ_API(fj_timer_start, struct fj_timer *timer, fj_time period)

FJ_API(fj_timer_stop, struct fj_timer *timer)

FJ_APIEX(fj_timer_get_remaining_time, fj_time, 0, struct fj_timer *timer)

FJ_API(fj_timer_tick_event, struct fj_timer *timer)


#endif
