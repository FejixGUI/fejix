/**
    \file
*/

#ifndef FEJIX_APP_H_INCLUDED
#define FEJIX_APP_H_INCLUDED


#include <fejix/core.h>


/** Event type */
enum fj_app_event_type {
    /** App has finished */
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


FJ_OPAQUE_OBJECT(fj_app_manager)

typedef fj_err (*fj_app_event_callback)(
    void *callback_data, enum fj_app_event_type type, void *opt_event_data);


FJ_METHOD_NONNULL(
    fj_app_create_manager,
    fj_err,
    struct fj_app_manager **out_app_manager,
    fj_app_event_callback callback,
    void *callback_data)

FJ_METHOD(fj_app_destroy_manager, fj_err, struct fj_app_manager *app_manager)

/** May never return! */
FJ_METHOD(fj_app_launched, fj_err, struct fj_app_manager *app_manager)

FJ_METHOD(fj_app_request_finish, fj_err, struct fj_app_manager *app_manager)

FJ_METHOD(fj_app_request_idle, fj_err, struct fj_app_manager *app_manager)


#endif
