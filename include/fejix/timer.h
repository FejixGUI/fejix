#ifndef FEJIX_TIMER_H_
#define FEJIX_TIMER_H_


#include <fejix/app.h>


enum fj_timer_precision {
    FJ_TIMER_PRECISION_HIGH,
    FJ_TIMER_PRECISION_NORMAL,
    FJ_TIMER_PRECISION_LOW,

    FJ_TIMER_PRECISION_ENUM32 = INT32_MAX,
};


FJ_SERVICE(fj_timer_service)
FJ_OBJECT(fj_timer)


FJ_API(fj_timer_service_new, struct fj_timer_service **out_service, struct fj_app *app)

FJ_API(fj_timer_service_del, struct fj_timer_service *service)

FJ_API(
    fj_timer_new,
    struct fj_timer **out_timer,
    struct fj_timer_service *service,
    enum fj_timer_precision precision)

FJ_API(fj_timer_del, struct fj_timer *timer)

FJ_API(fj_timer_start, struct fj_timer *timer, fj_time period)

FJ_API(fj_timer_stop, struct fj_timer *timer)

/** \returns #FJ_ERROR_INVALID_USAGE if the timer has not been started. */
FJ_API(fj_timer_get_remaining_time, struct fj_timer *timer, fj_time **out_time)

FJ_API_VOID(fj_timer_timeout_callback, struct fj_timer *timer)


#endif
