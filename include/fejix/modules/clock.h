/**
    \file
*/

#ifndef FJ_CLOCK_H_INCLUDED
#define FJ_CLOCK_H_INCLUDED


#include <fejix/modules/connection.h>


FJ_OBJECT_TYPE(fj_clock_manager)
FJ_OBJECT_TYPE(fj_clock)


/** \{ */
enum fj_clock_event_type {
    /** The regular clock event. */
    FJ_CLOCK_EVENT_TICK = 0,

    FJ_CLOCK_EVENT_ENUM32 = INT32_MAX,
};

struct fj_clock_event {
    enum fj_clock_event_type type;
    union fj_clock_event_data {
        struct fj_clock_event_tick {
            struct fj_clock *clock;
        } tick;
    } data;
};
/** \} */


FJ_CALLBACK_TYPE(
    fj_clock_event_callback,
    enum fj_error,
    struct fj_clock_manager *sender,
    struct fj_clock_event *event)


FJ_METHOD_NONNULL(
    fj_clock_create_manager,
    enum fj_error,
    struct fj_connection *conn,
    struct fj_clock_manager **out_manager)

FJ_METHOD(fj_clock_destroy_manager, enum fj_error, struct fj_clock_manager *manager)

FJ_METHOD(
    fj_clock_set_callback, void, struct fj_clock_manager *manager, fj_clock_event_callback callback)

FJ_METHOD(
    fj_clock_create,
    enum fj_error,
    struct fj_clock_manager *manager,
    fj_time interval,
    fj_time requested_precision,
    struct fj_clock **out_clock)

FJ_METHOD(fj_clock_destroy, enum fj_error, struct fj_clock_manager *manager, struct fj_clock *clock)

FJ_METHOD_LIST_BEGIN(fj_clock)
FJ_METHOD_LIST_ITEM(fj_clock_create_manager)
FJ_METHOD_LIST_ITEM(fj_clock_destroy_manager)
FJ_METHOD_LIST_ITEM(fj_clock_set_callback)
FJ_METHOD_LIST_ITEM(fj_clock_create)
FJ_METHOD_LIST_ITEM(fj_clock_destroy)
FJ_METHOD_LIST_END()

#endif
