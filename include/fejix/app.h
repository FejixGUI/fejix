/**
    \file
*/

#ifndef FEJIX_APP_H_
#define FEJIX_APP_H_


#include <fejix/base.h>


struct fj_app;
struct fj_app_system_data;

enum fj_app_message_type {
    FJ_APP_INIT,
    FJ_APP_DEINIT,
    FJ_APP_RUN,
    FJ_APP_QUIT,
    FJ_APP_PING,

    /**
        Wakes up an application that is waiting for events.
        This asks the system to send a custom event that gets handled with fj_app_ping_callback().

        This can be called from another thread provided that the app is not being destroyed.

        This is not supposed to be called periodically from the main thread as this goes through an
        inefficient process of communicating with the system.
        To regularly invoke a callback use a timer with zero timeout period.
    */
    FJ_APP_PING_EVENT,
    FJ_APP_START_EVENT,
    FJ_APP_QUIT_EVENT,
    FJ_APP_ACTIVATE_EVENT,
    FJ_APP_DEACTIVATE_EVENT,
    FJ_APP_SUSPEND_EVENT,
    FJ_APP_RESUME_EVENT,

    FJ_APP_REQUEST_MAX,
    FJ_APP_REQUEST_ENUM32 = INT32_MAX,
};

struct fj_app_init_message {
    void *extra_data;
};

union fj_app_message {
    struct fj_app_init_message *init;
    void *deinit;
    void *run;
    void *quit;
    void *ping;
    void *ping_event;
};

typedef enum fj_status (*fj_app_dispatcher)(
    struct fj_app *app, enum fj_app_message_type type, union fj_app_message message);

struct fj_app {
    /** The app's dispatcher that handles all the messages. */
    fj_app_dispatcher dispatch;

    /** The default dispatcher provided by the platform. This field is for convenience. */
    fj_app_dispatcher dispatch_default;

    /** The user's callback data. */
    void *custom_data;

    uintptr_t system_id;
    struct fj_app_system_data *system_data;
};


#endif
