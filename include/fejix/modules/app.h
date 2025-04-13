/**
    \file
*/

#ifndef FEJIX_APP_H_INCLUDED
#define FEJIX_APP_H_INCLUDED


#include <fejix/core.h>


/** App event type */
enum fj_app_event_type {
    /** App is finishing. */
    FJ_APP_EVENT_FINISH = 0,
    /** */
    FJ_APP_EVENT_IDLE = 1,
    /** */
    FJ_APP_EVENT_PAUSE = 2,
    /** */
    FJ_APP_EVENT_HIBERNATE = 3,
    /** */
    FJ_APP_EVENT_LOW_MEMORY = 4,

    FJ_APP_EVENT_ENUM32 = INT32_MAX,
};


FJ_OPAQUE_STRUCT_WITH_USERDATA(fj_app_manager)

typedef fj_err (*fj_app_event_callback)(
    struct fj_app_manager *app_manager, enum fj_app_event_type type, void *opt_event_data);


FJ_METHOD_NONNULL(
    fj_app_create_manager,
    fj_err,
    fj_app_event_callback event_callback,
    struct fj_app_manager **out_app_manager)

FJ_METHOD(fj_app_destroy_manager, fj_err, struct fj_app_manager *app_manager)

/** May never return! */
FJ_METHOD(fj_app_launched, fj_err, struct fj_app_manager *app_manager)

/** May be disrespected. */
FJ_METHOD(fj_app_finished, fj_err, struct fj_app_manager *app_manager)

/** Thread-safe. */
FJ_METHOD(fj_app_request_idle, fj_err, struct fj_app_manager *app_manager)


FJ_METHOD_LIST_BEGIN(fj_app)
FJ_METHOD_LIST_ITEM(fj_app_create_manager)
FJ_METHOD_LIST_ITEM(fj_app_destroy_manager)
FJ_METHOD_LIST_ITEM(fj_app_launched)
FJ_METHOD_LIST_ITEM(fj_app_finished)
FJ_METHOD_LIST_ITEM(fj_app_request_idle)
FJ_METHOD_LIST_END()


#endif
