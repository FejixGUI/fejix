/**
    \file
*/

#ifndef FJ_IO_THREAD_TIMER_H_INCLUDED
#define FJ_IO_THREAD_TIMER_H_INCLUDED


#include <fejix/modules/io_thread.h>


FJ_OBJECT_TYPE(fj_io_thread_timer_manager)
FJ_OBJECT_TYPE(fj_io_thread_timer)


/** \{ */
enum fj_io_thread_timer_event_type {
    /** The regular timer event. */
    FJ_IO_THREAD_TIMER_EVENT_TIMEOUT,

    FJ_IO_THREAD_TIMER_EVENT_ENUM32 = INT32_MAX,
};

struct fj_io_thread_timer_event_timeout {
    struct fj_io_thread_timer *timer;
};

union fj_io_thread_timer_event_data {
    struct fj_io_thread_timer_event_timeout *timeout;
};
/** \} */


FJ_CALLBACK_TYPE(
    fj_io_thread_timer_event_callback,
    enum fj_error,
    struct fj_io_thread_timer_manager *event_object,
    enum fj_io_thread_timer_event_type event_type,
    union fj_io_thread_event_data event_data)


FJ_METHOD_WITH_FALLBACK(
    fj_io_thread_timer_create_manager,
    enum fj_error,
    FJ_ERROR_UNIMPLEMENTED,
    struct fj_io_thread *io_thread,
    struct fj_io_thread_timer_manager **out_manager)

FJ_METHOD(
    fj_io_thread_timer_destroy_manager, enum fj_error, struct fj_io_thread_timer_manager *manager)

FJ_METHOD(
    fj_io_thread_timer_set_callback,
    void,
    struct fj_io_thread_timer_manager *manager,
    fj_io_thread_timer_event_callback callback)

FJ_METHOD(
    fj_io_thread_timer_create,
    enum fj_error,
    struct fj_io_thread_timer_manager *manager,
    fj_time initial_delay,
    fj_time repeat_period,
    fj_time requested_precision,
    struct fj_io_thread_timer **out_timer)

FJ_METHOD(
    fj_io_thread_timer_destroy,
    enum fj_error,
    struct fj_io_thread_timer_manager *manager,
    struct fj_io_thread_timer *timer)

FJ_METHOD_LIST_BEGIN(fj_io_thread_timer)
FJ_METHOD_LIST_ITEM(fj_io_thread_timer_create_manager)
FJ_METHOD_LIST_ITEM(fj_io_thread_timer_destroy_manager)
FJ_METHOD_LIST_ITEM(fj_io_thread_timer_set_callback)
FJ_METHOD_LIST_ITEM(fj_io_thread_timer_create)
FJ_METHOD_LIST_ITEM(fj_io_thread_timer_destroy)
FJ_METHOD_LIST_END()

#endif
