/** \HEADER */

#ifndef FEJIX_APP_H_
#define FEJIX_APP_H_


#include <fejix/base.h>


enum fj_app_message
{
    /** Initializes the application.

        You must initialize all the fields except fj_app::main_task and
        fj_app::data *before* dispatching this message.*/
    FJ_APP_INIT,

    /** Deinitializes the application.

        This message is not intended for quitting the app as the quitting
        process can be complicated and on some platforms even asynchronous.
        Cancel fj_app::main_task instead (see #FJ_APP_DID_SET_TASK_FLAGS).

        All created objects should be destroyed before destroying the app. */
    FJ_APP_DEINIT,

    /** \param[in] flags Provides #fj_app_task_flags.
        \see fj_app::main_task, fj_app_task_flags */
    FJ_APP_ON_SET_LOOP_CAPS,

    /** Wakes up an application that is waiting for events.
        This asks the system to send a custom event that goes back as
        #FJ_APP_DID_PING.

        This can be sent from another thread provided that the app is not
        being destroyed.

        This is not supposed to be called periodically from the main thread as
        this goes through an inefficient process of communicating with the
        system. To regularly invoke a user callback on the main thread,
        use a timer with zero timeout period. */
    FJ_APP_PING,

    FJ_APP_ON_PING,
    FJ_APP_ON_ACTIVATE,
    FJ_APP_ON_DEACTIVATE,
    FJ_APP_ON_SUSPEND,
    FJ_APP_ON_RESUME,

    /** Provides an internal system handle of the app.
        This is sent on app initialization.

        \param[in] system_handle A pointer to a platform-dependent handle.

        \note
        - **X11**: the handle is the Xlib's `Display*`.
        - **Wayland**: the handle is `wl_display*`.
        - **Windows API**: the handle is the current `HINSTANCE`. */
    FJ_APP_ON_SET_SYSTEM_HANDLE,

    FJ_APP_MESSAGE_COUNT,
    FJ_APP_MESSAGE_ENUM_MAX = INT32_MAX,
};


/// \BEGIN{app_other}


enum fj_app_loop_caps
{
    FJ_APP_LOOP_CAPS_RUNNABLE = 1 << 0,
    FJ_APP_LOOP_CAPS_BREAKABLE = 1 << 1,
    FJ_APP_LOOP_CAPS_ITERATABLE = 1 << 2,
    FJ_APP_LOOP_CAPS_ENUM_MAX = INT32_MAX,
};

union fj_app_message_data
{
    enum fj_app_loop_caps loop_caps;
    uintptr_t system_handle;
    void *_unused;
};

struct fj_app;
typedef fj_err (*fj_app_dispatcher)(
    struct fj_app *app,
    enum fj_app_message message,
    union fj_app_message_data *data);


/// \END


/** This object manages the application lifecycle and all of its resources.

    \note
    This should generally be a instantiated once per program.
    Whether it can be instantiated multiple times is implementation-defined.
    - **Wayland**, **X11**: an application object can be instantiated as many
        times as you want.
    - **Windows API**: instantiating this multiple times is not a strict error
        if you are careful with requests that affect some global process state.

    \note
    This should generally be intantiated on the main thread.
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
{
    /** The app's message dispatcher function.

        Override this to handle user input and internal library events.
        This *must* call the default dispatcher for unhandled messages. */
    fj_app_dispatcher dispatch;

    /** The default dispatcher provided by the platform.

        This field is only for convenience, it must be set manually and is
        supposed to be called for unhandled messages in fj_app::dispatch. */
    fj_app_dispatcher dispatch_default;

    uintptr_t user_data;

    struct fj_app_private_data *data;
};


#endif
