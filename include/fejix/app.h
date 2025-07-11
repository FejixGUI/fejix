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
    fj_dispatcher dispatch;

    /** The default dispatcher provided by the platform.

        This field is only for convenience, it must be set manually and is
        supposed to be called for unhandled messages in fj_app::send. */
    fj_dispatcher dispatch_default;

    uintptr_t user_data;

    struct fj_app_private_data *data;

    /** This is the user's task that is polled and canceled by the
        fj_app::main_task.

        This can be used to execute code at the start, at the end, and at
        each iteration of the event loop.
        Completing this task has no affect on the event loop,
        to quit the app, cancel fj_app::main_task (in case it's supported).

        \noop TODO Is the order of user_task.poll() and processing the events
        \noop   important?  */
    struct fj_task user_task;

    /** The event loop task.

        There are several ways of running the app depending on the platform
        and your limitations:

        1. *Exiting the program*.

            Yes, exiting the main function right after the
            initialization. This works on platforms like WebAssembly where
            your code is basically a library that is run by a wrapper like the
            actual JavaScript program.

            This method *must be* used when the #FJ_APP_TASK_FLAG_AWAITABLE
            is *not* set in the #FJ_APP_DID_SET_TASK_FLAGS message.

        2. *Awaiting the main task*.

            This means calling #FJ_APP_AWAIT with no arguments \---
            fj_app::main_task is always awaited with whatever task you specify
            in order to actually run the tasks that depend on the event loop.

            This is the default and recommended method for cases when
            #FJ_APP_TASK_FLAG_AWAITABLE is set in
            the #FJ_APP_DID_SET_TASK_FLAGS message.

        3. *Manually polling the main task*.

            This gives you the ability to run a legacy-style event loop like
            the ones in e.g. SDL2, GLFW and Raylib.
            This approach is not portable and can have quite unexpected
            behaviors on some platforms.

            This is possible if #FJ_APP_TASK_FLAG_POLLABLE is set in
            the #FJ_APP_DID_SET_TASK_FLAGS message.

        \see fj_app_task_flags, FJ_APP_DID_SET_TASK_FLAGS */
    struct fj_task main_task;
};

/// \END


/// \BEGIN{app_other}

/** \see fj_app::main_task */
enum fj_app_task_flags
{
    /** Indicates that the app supports the #FJ_APP_AWAIT message.
        If not present, means that the app is run externally (e.g. when it is
        embedded into the actual executable that handles the event processing),
        which means that the app's entrypoint should return as soon as it
        finishes initializing the app in order for the app to start running.

        \note
        - **WebAssembly**: unsupported; you should exit after initializing
            the application in order for JavaScript code to run it.*/
    FJ_APP_TASK_FLAG_AWAITABLE = 1 << 0,

    /** Indicates that fj_app::main_task is cancelable.
        If not present, means that canceling fj_app::main_task has no effect,
        which happens on platforms where the application is managed by the
        system and cannot quit whenever it wants, but when the system decides.

        \note
        - **Wayland**, **X11**, **Windows API**: supported.
        - **UIKit**: unsupported, though it is in theory possible to exit
            the program by calling the exit function. This, however, is
            [considered a bad practice by Apple
            ](https://developer.apple.com/library/archive/qa/qa1561/_index.html).
        - **WASM**: unsupported. */
    FJ_APP_TASK_FLAG_CANCELABLE = 1 << 1,

    /** This flag is only meaningful when #FJ_APP_TASK_FLAG_AWAITABLE is
        present.

        Indicates that the app can be run just by polling fj_app::main_task in a
        loop. If not present, means that to run the app you need to await
        its task.

        \note
        - **Wayland**, **X11**: supported.
        - **Windows API**: supported with a platform-specific behavior:
            when the user starts resizing a window, the main task freezes
            in its fj_task::poll method. This is because the
            [DefWindowProc
            ](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-defwindowprocw)
            function implements all the window resizing logic in
            a client-side event loop,
            which continues to process asynchronous tasks,
            but blocks until the resizing is finished.
            If you implement drawing inside a custom event loop that polls
            the main task, it will result in freezes during window
            resizing.
        - **UIKit**: unsupported, but usable with a platform-specific
            limitation: even though the pollable flag is never specified, you
            can poll the main task to process events, but only from within
            a blocking call to #FJ_APP_AWAIT.
            The reason is that the toplevel call to #FJ_APP_AWAIT
            calls `UIApplicationMain()` that is necessary to run the app,
            but inside it manual polling is allowed.*/
    FJ_APP_TASK_FLAG_POLLABLE = 1 << 2,

    _fj_app_task_flags_ensure_int32 = INT32_MAX,
};

/// \END


/// \BEGIN{app_messages}

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

    /** Awaits until a task completes by processing system events.

        \param[in] task (optional) A pointer to a ::fj_task to await.

        This always polls for fj_app::main_task to continue processing events,
        thus specifying the task parameter is not necessary.
        Awaiting will always finish when the application task gets canceled.

        The support for this message and its exact behavior is
        platform-defined.

        \see fj_app::main_task, fj_app_task_flags */
    FJ_APP_AWAIT,

    /** \param[in] flags Provides #fj_app_task_flags.
        \see fj_app::main_task, fj_app_task_flags */
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

        \param[in] system_handle A pointer to a platform-dependent handle.

        \note
        - **X11**: the handle is the Xlib's `Display*`.
        - **Wayland**: the handle is `wl_display*`.
        - **Windows API**: the handle is the current `HINSTANCE`. */
    FJ_APP_DID_SET_SYSTEM_HANDLE,

    FJ_APP_MESSAGE_MAX,

    _fj_app_message_ensure_int32 = INT32_MAX,
};


/// \END


#endif
