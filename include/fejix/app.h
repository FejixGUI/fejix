/** \HEADER */

#ifndef FEJIX_APP_H_
#define FEJIX_APP_H_


#include <fejix/base.h>


/// \BEGIN{app_definition}

/** This object manages the application lifecycle and all of its resources.

    \note
    This should generally be a instantiated once per program.
    Whether it can be instantiated multiple times is implementation-defined.
    - **Wayland**, **X11**: an application object can be instantiated as many
        times as you want.
    - **WindowsAPI**: instantiating this multiple times is not a strict error
        if you are careful with requests that affect some global process state.

    \note
    This should generally be intantiated on the main thread.
    Whether it can be instantiated on other threads is platform-defined.
    - **UIKit**: this *must* be instatiated on the main thread and all messsages
        where thread-safety is not specified *must* be sent from the main
        thread.
    - **WindowsAPI**, **Wayland**, **X11**: this can be instantiated on any
        thread.

    \note
    This should generally be run on the same thread where instantiated.
    Whether or not it can be run on other threads is platform-defined.
    - **Wayland**, **X11**: this can be run from any thread.
    - **WindowsAPI**: there are restrictions that limit event handling to the
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
    fj_dispatcher dispatch;

    /** The default dispatcher provided by the platform.

        This field is only for convenience, it must be set manually and is
        supposed to be called for unhandled messages in fj_app::send. */
    fj_dispatcher dispatch_default;

    uintptr_t user_data;

    struct fj_app_private_data *data;

    /** This is the user's task that is polled and canceled by the
        fj_app::main_task. Polling happens once per fj_app::main_task's poll,
        most likely before processing events. Canceling happens when the
        fj_app::main_task gets canceled.

        This can be used to execute code at the start, at the end, and at
        each iteration of the event loop.

        Completing this task has no affect on the event loop,
        to quit the app, cancel fj_app::main_task (in case it's supported). */
    struct fj_task user_task;

    /** The event loop task. \noop TODO app execution model docs  */
    struct fj_task main_task;
};

/// \END


/// \BEGIN{app_other}

enum fj_app_task_flags
{
    /** Indicates that the app supports the #FJ_APP_AWAIT message.
        If not present, means that the app is run externally (e.g. when it is
        embedded into the actual executable that handles the event processing),
        which means that the app's entrypoint should return as soon as it
        finishes initializing the app in order for the app to start running. */
    FJ_APP_TASK_FLAG_AWAITABLE = 1 << 0,

    /** Indicates that fj_app::main_task is cancelable.
        If not present, means that canceling fj_app::main_task has no effect,
        which happens on platforms where the application is managed by the
        system and cannot quit whenever it wants, but when the system decides.

        \note
        - **UIKit**: the app task is not supposed to be cancelable.
            The only way to forcefully exit the program is to call the exit
            function, which is [considered a bad practice by Apple
            ](https://developer.apple.com/library/archive/qa/qa1561/_index.html).
        - **WASM**: the app task is not cancelable.
        */
    FJ_APP_TASK_FLAG_CANCELABLE = 1 << 1,

    /** This flag is only meaningful when #FJ_APP_TASK_FLAG_AWAITABLE is
        present.

        Indicates that the app can be run just by polling fj_app::main_task in a
        loop. If not present, means that to run the app you need to await
        its task.

        \note
        - **UIKit**: the pollable flag is never specified as you need to enter
            the await function to run the app, but it is in fact pollable
            when inside that function. The reason is that the toplevel call
            to #FJ_APP_AWAIT runs `UIApplicationMain()` which allows the app
            to run, but inside it other mechanisms are available for running
            the app by just polling in a loop. */
    FJ_APP_TASK_FLAG_POLLABLE = 1 << 2,

    _fj_app_task_flags_ensure_int32 = INT32_MAX,
};

/// \END


/// \BEGIN{app_messages}

enum fj_app_message
{
    /** Initializes the application. */
    FJ_APP_INIT,

    /** Deinitializes the application.

        This message is not intended for quitting the app as the quitting
        process can be complicated and on some platforms even asynchronous.
        Cancel fj_app::main_task instead (see #FJ_APP_DID_SET_TASK_FLAGS).

        All created objects should be destroyed before destroying the app. */
    FJ_APP_DEINIT,

    /** Awaits until a task completes by processing system events.

        \param[in] task (optional) A pointer to a ::fj_task to await.

        This always polls for fj_app::main_task to continue processing events,
        thus specifying the task parameter is not necessary.
        Awaiting will always finish when the application task gets canceled.

        The support for this message and its exact behavior is
        platform-defined. See #FJ_APP_DID_SET_TASK_FLAGS. */
    FJ_APP_AWAIT,

    /** \noop TODO app execution model docs

        \param[in] flags Provides #fj_app_task_flags. */
    FJ_APP_DID_SET_TASK_FLAGS,

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

    FJ_APP_DID_PING,
    FJ_APP_DID_ACTIVATE,
    FJ_APP_DID_DEACTIVATE,
    FJ_APP_DID_SUSPEND,
    FJ_APP_DID_RESUME,

    /** Provides an internal system handle of the app.
        This is sent on app initialization.

        \param[in] system_handle Platform-defined.

        \note
        - **X11**: the handle is the Xlib's `Display*`.
        - **Wayland**: the handle is `wl_display`.
        - **Windows API*: the handle is the current `HINSTANCE`. */
    FJ_APP_DID_SET_SYSTEM_HANDLE,

    FJ_APP_MESSAGE_MAX,

    _fj_app_message_ensure_int32 = INT32_MAX,
};


/// \END


#endif
