/**
    \file
*/

#ifndef FEJIX_APP_H_
#define FEJIX_APP_H_


#include <fejix/base.h>


struct fj_app;
struct fj_app_data;

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
    FJ_APP_ON_PING,
    FJ_APP_ON_START,
    FJ_APP_ON_QUIT,
    FJ_APP_ON_ACTIVATE,
    FJ_APP_ON_DEACTIVATE,
    FJ_APP_ON_SUSPEND,
    FJ_APP_ON_RESUME,

    /** Supplies the system handle for the app. */
    FJ_APP_ON_SET_SYSTEM_HANDLE,

    FJ_APP_REQUEST_MAX,
    FJ_APP_REQUEST_ENUM32 = INT32_MAX,
};

struct fj_app_init_message {
    void *extra_data;
};

struct fj_app_on_set_system_handle_message {
    uintptr_t system_handle;
};

// TODO transform everything into a task and add a default waiting function
union fj_app_message {
    struct fj_app_init_message const *init;
    void *deinit;
    void *run;
    void *quit;
    void *ping;
    void *on_ping;
    struct fj_app_on_set_system_handle_message const *on_set_system_handle;
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

    struct fj_app_data *data;
};


#endif
