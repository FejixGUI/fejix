/**
    \file
*/

#ifndef FEJIX_CONNECTION_H_INCLUDED
#define FEJIX_CONNECTION_H_INCLUDED


#include <fejix/core.h>


FJ_OBJECT(fj_connection)


/** \{ */
enum fj_connection_event_type {
    /** Generated on every event loop iteration before going to sleep. */
    FJ_CONNECTION_EVENT_IDLE = 0,
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


typedef enum fj_error (*fj_connection_event_callback)(
    struct fj_connection *connection, struct fj_connection_event *event);


FJ_METHOD_NONNULL(fj_connection_create, enum fj_error, struct fj_connection **out_connection)

FJ_METHOD(fj_connection_destroy, enum fj_error, struct fj_connection *connection)

FJ_METHOD(
    fj_connection_set_callback,
    void,
    struct fj_connection *connection,
    fj_connection_event_callback callback)

/** May never return! */
FJ_METHOD(fj_connection_run, enum fj_error, struct fj_connection *connection)

/** May be disrespected. */
FJ_METHOD(fj_connection_notify_finish, enum fj_error, struct fj_connection *connection)

/** Thread-safe. */
FJ_METHOD(fj_connection_prevent_sleep, enum fj_error, struct fj_connection *connection)


FJ_METHOD_LIST_BEGIN(fj_connection)
FJ_METHOD_LIST_ITEM(fj_connection_create)
FJ_METHOD_LIST_ITEM(fj_connection_destroy)
FJ_METHOD_LIST_ITEM(fj_connection_set_callback)
FJ_METHOD_LIST_ITEM(fj_connection_run)
FJ_METHOD_LIST_ITEM(fj_connection_notify_finish)
FJ_METHOD_LIST_ITEM(fj_connection_prevent_sleep)
FJ_METHOD_LIST_END()


#endif
