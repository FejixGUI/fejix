/**
    \file
*/

#ifndef FEJIX_CONNECTION_H_INCLUDED
#define FEJIX_CONNECTION_H_INCLUDED


#include <fejix/core.h>


FJ_OBJECT_TYPE(fj_connection)
FJ_OBJECT_TYPE(fj_connection_timeout)


/** \{ */
enum fj_connection_event_type {
    /** Sent every time the conneciton has dispatched all events and may be going to sleep. */
    FJ_CONNECTION_EVENT_DISPATCHED = 0,
    /** Connection is finishing. */
    FJ_CONNECTION_EVENT_FINISH = 1,
    /** */
    FJ_CONNECTION_EVENT_PAUSE = 2,
    /** */
    FJ_CONNECTION_EVENT_HIBERNATE = 3,
    /** */
    FJ_CONNECTION_EVENT_LOW_MEMORY = 4,

    FJ_CONNECTION_EVENT_ENUM32 = INT32_MAX,
};

struct fj_connection_event {
    enum fj_connection_event_type type;
    union fj_connection_event_data {
        void *_unused;
    } data;
};
/** \} */


FJ_CALLBACK_TYPE(
    fj_connection_event_callback,
    enum fj_error,
    struct fj_connection *sender,
    struct fj_connection_event *event)


FJ_METHOD_WITH_FALLBACK(
    fj_connection_create,
    enum fj_error,
    FJ_ERROR_UNIMPLEMENTED,
    struct fj_connection **out_connection)

FJ_METHOD(fj_connection_destroy, enum fj_error, struct fj_connection *connection)

FJ_METHOD(
    fj_connection_set_callback,
    void,
    struct fj_connection *connection,
    fj_connection_event_callback callback)

/** May never return! */
FJ_METHOD(fj_connection_dispatch, enum fj_error, struct fj_connection *connection)

/** Thread-safe, may be disrespected. */
FJ_METHOD(fj_connection_idle, enum fj_error, struct fj_connection *connection)

/** May be disrespected. */
FJ_METHOD(fj_connection_finish, enum fj_error, struct fj_connection *connection)

FJ_METHOD_LIST_BEGIN(fj_connection)
FJ_METHOD_LIST_ITEM(fj_connection_create)
FJ_METHOD_LIST_ITEM(fj_connection_destroy)
FJ_METHOD_LIST_ITEM(fj_connection_set_callback)
FJ_METHOD_LIST_ITEM(fj_connection_dispatch)
FJ_METHOD_LIST_ITEM(fj_connection_dispatch_continue_sync)
FJ_METHOD_LIST_ITEM(fj_connection_dispatch_continue_async)
FJ_METHOD_LIST_END()


#endif
