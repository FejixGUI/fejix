/**
    \file
*/

#ifndef FJ_TIMER_H_INCLUDED
#define FJ_TIMER_H_INCLUDED


#include <fejix/modules/connection.h>


FJ_OBJECT_TYPE(fj_timer_manager)
FJ_OBJECT_TYPE(fj_timer)


/** \{ */
enum fj_timer_event_type {
    /** The regular timer event. */
    FJ_TIMER_EVENT_TIMEOUT = 0,

    FJ_TIMER_EVENT_ENUM32 = INT32_MAX,
};

struct fj_timer_event {
    enum fj_timer_event_type type;
    union fj_timer_event_data {
        struct fj_timer_event_timeout {
            struct fj_timer *timer;
        } timeout;
    } data;
};
/** \} */


FJ_CALLBACK_TYPE(
    fj_timer_event_callback,
    enum fj_error,
    struct fj_timer_manager *sender,
    struct fj_timer_event *event)


FJ_METHOD_WITH_FALLBACK(
    fj_timer_create_manager,
    enum fj_error,
    FJ_ERROR_UNIMPLEMENTED,
    struct fj_connection *conn,
    struct fj_timer_manager **out_manager)

FJ_METHOD(fj_timer_destroy_manager, enum fj_error, struct fj_timer_manager *manager)

FJ_METHOD(
    fj_timer_set_callback, void, struct fj_timer_manager *manager, fj_timer_event_callback callback)

FJ_METHOD(
    fj_timer_create,
    enum fj_error,
    struct fj_timer_manager *manager,
    fj_time interval,
    fj_time requested_precision,
    struct fj_timer **out_timer)

FJ_METHOD(fj_timer_destroy, enum fj_error, struct fj_timer_manager *manager, struct fj_timer *timer)

FJ_METHOD_LIST_BEGIN(fj_timer)
FJ_METHOD_LIST_ITEM(fj_timer_create_manager)
FJ_METHOD_LIST_ITEM(fj_timer_destroy_manager)
FJ_METHOD_LIST_ITEM(fj_timer_set_callback)
FJ_METHOD_LIST_ITEM(fj_timer_create)
FJ_METHOD_LIST_ITEM(fj_timer_destroy)
FJ_METHOD_LIST_END()

#endif
