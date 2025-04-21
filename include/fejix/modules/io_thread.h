/**
    \file
*/

#ifndef FEJIX_IO_THREAD_H_INCLUDED
#define FEJIX_IO_THREAD_H_INCLUDED


#include <fejix/core.h>


FJ_OBJECT_TYPE(fj_io_thread)


/** \{ */
enum fj_io_thread_event_type {
    /** Sent every time the conneciton has dispatched all events and may be going to sleep. */
    FJ_IO_THREAD_EVENT_NO_MORE_EVENTS = 0,
    /** Connection is finishing. */
    FJ_IO_THREAD_EVENT_FINISH = 1,
    /** */
    FJ_IO_THREAD_EVENT_PAUSE = 2,
    /** */
    FJ_IO_THREAD_EVENT_HIBERNATE = 3,
    /** */
    FJ_IO_THREAD_EVENT_LOW_MEMORY = 4,

    FJ_IO_THREAD_EVENT_ENUM32 = INT32_MAX,
};

struct fj_io_thread_event {
    enum fj_io_thread_event_type type;
    union fj_io_thread_event_data {
        void *_unused;
    } data;
};
/** \} */


FJ_CALLBACK_TYPE(
    fj_io_thread_event_callback,
    enum fj_error,
    struct fj_io_thread *sender,
    struct fj_io_thread_event *event)


FJ_METHOD_WITH_FALLBACK(
    fj_io_thread_create, enum fj_error, FJ_ERROR_UNIMPLEMENTED, struct fj_io_thread **out_io_thread)

FJ_METHOD(fj_io_thread_destroy, enum fj_error, struct fj_io_thread *io_thread)

FJ_METHOD(
    fj_io_thread_set_callback,
    void,
    struct fj_io_thread *io_thread,
    fj_io_thread_event_callback callback)

/** May never return! */
FJ_METHOD(fj_io_thread_start, enum fj_error, struct fj_io_thread *io_thread)

/** Thread-safe, may be disrespected. */
FJ_METHOD(fj_io_thread_ping_self, enum fj_error, struct fj_io_thread *io_thread)

/** May be disrespected. */
FJ_METHOD(fj_io_thread_finish, enum fj_error, struct fj_io_thread *io_thread)

FJ_METHOD_LIST_BEGIN(fj_io_thread)
FJ_METHOD_LIST_ITEM(fj_io_thread_create)
FJ_METHOD_LIST_ITEM(fj_io_thread_destroy)
FJ_METHOD_LIST_ITEM(fj_io_thread_set_callback)
FJ_METHOD_LIST_ITEM(fj_io_thread_start)
FJ_METHOD_LIST_ITEM(fj_io_thread_ping_self)
FJ_METHOD_LIST_ITEM(fj_io_thread_finish)
FJ_METHOD_LIST_END()


#endif
