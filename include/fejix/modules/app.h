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


/**
    This must be shown!
*/
FJ_DEFINE_OBJECT(fj_app)

/**
    Wow
*/
struct test {
    /** Wow */
    int x;
};


// fejix-autogen methods since 0.1.0

FJ_PUBLIC
fj_err (*fj_app_create_manager)(
    struct fj_app **out_app_manager,
    fj_err (*event_callback)(
        void *callback_data, enum fj_app_event_type type, void *opt_event_data),
    void *callback_data);

FJ_PUBLIC
fj_err (*fj_app_destroy_manager)(struct fj_app *app_manager);

/** May never return! */
FJ_PUBLIC
fj_err (*fj_app_launched)(struct fj_app *app_manager);

FJ_PUBLIC
fj_err (*fj_app_request_finish)(struct fj_app *app_manager);

FJ_PUBLIC
fj_err (*fj_app_request_idle)(struct fj_app *app_manager);


#endif
