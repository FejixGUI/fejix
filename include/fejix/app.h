/** \file

    TODO running the app... */

#ifndef FEJIX_APP_H_
#define FEJIX_APP_H_


#include <fejix/base.h>


struct fj_app;


/// \addtogroup app_utils
/// \{

enum fj_app_run_caps
{
    FJ_APP_RUN_CAPS_RUNNABLE = 1 << 0,
    FJ_APP_RUN_CAPS_QUITTABLE = 1 << 1,
    FJ_APP_RUN_CAPS_ITERABLE = 1 << 2,
    FJ_APP_RUN_CAPS_AWAITABLE = 1 << 3,

    FJ_APP_RUN_CAPS_ENUM_MAX = INT32_MAX,
};

/// \}


/// \addtogroup app_dispatching
/// \{

enum fj_app_message_type
{
    FJ_APP_INIT,
    FJ_APP_DEINIT,
    FJ_APP_ON_SET_RUN_CAPS,
    FJ_APP_RUN,
    FJ_APP_QUIT,
    FJ_APP_ITERATE,
    FJ_APP_AWAIT,
    FJ_APP_ON_ITERATE,
    FJ_APP_ON_QUIT,
    FJ_APP_PING,
    FJ_APP_ON_PING,
    FJ_APP_ON_ACTIVATE,
    FJ_APP_ON_DEACTIVATE,
    FJ_APP_ON_SUSPEND,
    FJ_APP_ON_RESUME,
    FJ_APP_ON_SET_SYSTEM_HANDLE,

    FJ_APP_MESSAGE_COUNT,
    FJ_APP_MESSAGE_ENUM_MAX = INT32_MAX,
};

/** \param[inout] message (optional)
        This can be NULL for messages that are empty or are explicitly
        documented to accept NULL. */
typedef fj_err (*fj_app_dispatcher)(
    struct fj_app *app, enum fj_app_message_type message_type, void *message);

/// \}


/// \addtogroup app_definition
/// \{

struct fj_app_base
{
    /** This is intended to be overridden by the user to provide a custom
        dispatcher for specific message types. */
    fj_app_dispatcher dispatch;

    /** This is optional and is not used by the library in any way.

        This is for your convenience to store the default platform dispatcher
        that you should call for messages that are not handled by the
        fj_app_base::dispatch. */
    fj_app_dispatcher dispatch_default;

    /** This is optional and is not used by the library in any way.

        This is for your convenience to store your own data. */
    void *user_data;
};

/** This object manages the application lifecycle and all of its resources.

    \note
    This should generally be a instantiated once per program.
    Whether it can be instantiated multiple times is implementation-defined.
    - **Windows API**, **Wayland**, **X11**: an application object can be
        instantiated as many times as you want.

    \note
    This should generally be instantiated on the main thread.
    Whether it can be instantiated on other threads is platform-defined.
    - **UIKit**: this *must* be instatiated on the main thread and all messsages
        where thread-safety is not specified *must* be sent from the main
        thread.
    - **Windows API**, **Wayland**, **X11**: this can be instantiated on any
        thread.

    \note
    This should generally be run on the same thread where instantiated.
    Whether or not it can be run on other threads is platform-defined.
    - **Wayland**, **X11**: this can be run from any thread.
    - **Windows API**: there are restrictions that limit event handling to the
        thread that has created all the objects that may receive events.
        This is because [event queues are thread-local
        ](https://learn.microsoft.com/en-us/windows/win32/winmsg/using-messages-and-message-queues)
        and windows are [bound to thread event queues at creation
        ](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-peekmessagew).
        Because of this, it is better to create the app and all other objects
        on the same thread that handles them. */
struct fj_app
#ifndef FJ_OPT_PRIVATE
{
    struct fj_app_base base;
}
#endif
;

/// \}


/// \addtogroup app_messages
/// \{

/** Initializes the application.

    This can dispatch some initial events, they should be handled
    immediately. */
struct fj_app_init_message
{
    /** \returns A newly created app, undefined on failure.
            To check for errors, never check the returned pointer for NULL,
            handle the return value. */
    struct fj_app *out_app;

    /** This is taken as the base of the app. */
    struct fj_app_base base;
};

/** Deinitializes the application.

    This message is not intended for quitting the app as the quitting
    process can be complicated and on some platforms even asynchronous.
    Use ::fj_app_quit_message instead.

    All created objects should be destroyed before destroying the app.
 */
struct fj_app_deinit_message
{
    uint8_t _empty;
};

struct fj_app_on_set_run_caps_message
{
    enum fj_app_run_caps const caps;
};

struct fj_app_run_message
{
    uint8_t _empty;
};

struct fj_app_quit_message
{
    uint8_t _empty;
};

struct fj_app_iterate_message
{
    uint8_t _empty;
};

struct fj_app_await_message
{
    uint8_t _empty;
};

struct fj_app_on_iterate_message
{
    uint8_t _empty;
};

struct fj_app_on_quit_message
{
    uint8_t _empty;
};


/** Wakes up an application that is waiting for events.
    This asks the system to send a custom event that goes back as
    fj_app_message::on_ping.

    This can be sent from another thread provided that the app is not
    being destroyed.

    This is not supposed to be called periodically from the main thread
    as this goes through an inefficient process of communicating with the
    system. To regularly invoke a user callback on the main thread,
    use a timer with zero timeout period. */
struct fj_app_ping_message
{
    uint8_t _empty;
};

struct fj_app_on_ping_message
{
    uint8_t _empty;
};

struct fj_app_on_activate_message
{
    uint8_t _empty;
};

struct fj_app_on_deactivate_message
{
    uint8_t _empty;
};

struct fj_app_on_suspend_message
{
    uint8_t _empty;
};

struct fj_app_on_resume_message
{
    uint8_t _empty;
};

/** Provides an internal system handle of the app.
    This is sent on app initialization.

    \note
    - **X11**: the handle is the Xlib's `Display*`.
    - **Wayland**: the handle is `wl_display*`.
    - **Windows API**: the handle is the current `HINSTANCE`. */
struct fj_app_on_set_system_handle_message
{
    uintptr_t handle;
};

/// \}


#endif
