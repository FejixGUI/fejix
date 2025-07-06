/** \HEADER
    To run your program you need to create and run the app object. */

#ifndef FEJIX_APP_H_
#define FEJIX_APP_H_


#include <fejix/base.h>


/// \BEGIN{app_messages}


/** Initializes the application. */
struct fj_app_init_message
{
    /** This is for future compatibility. Set to NULL. */
    void *extra_data;
};

struct fj_app_run_message
{
    uint8_t _noargs;
};

struct fj_app_quit_message
{
    uint8_t _noargs;
};

struct fj_app_start_notify_message
{
    uint8_t _noargs;
};
struct fj_app_quit_notify_message
{
    uint8_t _noargs;
};

/** Supplies the system handle for the app. */
struct fj_app_system_handle_notify_message
{
    uintptr_t system_handle;
};

/** Wakes up an application that is waiting for events.
    This asks the system to send a custom event that goes back as
    #fj_app_ping_notify_message.

    This can be called from another thread provided that the app is not
    being destroyed.

    This is not supposed to be called periodically from the main thread as
    this goes through an inefficient process of communicating with the
    system. To regularly invoke a callback, use a timer with zero timeout
    period. */
struct fj_app_ping_message
{
    uint8_t _noargs;
};

struct fj_app_ping_notify_message
{
    uint8_t _noargs;
};

/** Waits for all the tasks to complete.

    This typically involves running a message loop.
    The message loop functionality and therefore the support and specific
    behavior of this request is platform-defined.

    \note
    The support of this message as well as the support for each individual
    task type for waiting is platform-defined.
    - X11, Wayland: this is compatible with all possible tasks. */
struct fj_app_sync_wait_message
{
    // TODO
};

enum fj_app_message_type
{

    FJ_APP_INIT,
    FJ_APP_DEINIT,
    FJ_APP_RUN,
    FJ_APP_QUIT,
    FJ_APP_PING,
    FJ_APP_PING_NOTIFY,
    FJ_APP_START_NOTIFY,
    FJ_APP_QUIT_NOTIFY,
    FJ_APP_ACTIVATE_NOTIFY,
    FJ_APP_DEACTIVATE_NOTIFY,
    FJ_APP_SUSPEND_NOTIFY,
    FJ_APP_RESUME_NOTIFY,
    FJ_APP_SYSTEM_HANDLE_NOTIFY,
    FJ_APP_SYNC_WAIT,

    FJ_APP_MESSAGE_TYPE_MAX,
    FJ_APP_MESSAGE_TYPE_ENSURE_INT32 = INT32_MAX,
};


/// \END

/// \BEGIN{app_definition}

struct fj_app
{
    /** The app's dispatcher that handles all the messages. */
    fj_dispatcher dispatch;

    /** The default dispatcher provided by the platform. This field is for
        convenience. */
    fj_dispatcher dispatch_default;

    /** The user's callback data. */
    uintptr_t user_data;

    struct fj_app_private_data *private;
};

/// \END

#endif
