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
    /** The app's message sender function. */
    fj_sender send;

    /** The default sender provided by the platform. This field is only for
        convenience. */
    fj_sender _send;

    /** The user's callback data. */
    uintptr_t data;

    struct fj_app_private_data *_data;
};

/// \END


/// \BEGIN{app_other}

enum fj_app_loop_flags
{
    /** Indicates that #FJ_APP_LOOP can be run from the program's entrypoint to
        run the application. If not present, means that the app is run by some
        other mechanism, externally, not from within the main function and
        definitely not using the #FJ_APP_LOOP message. */
    FJ_APP_LOOP_FLAG_TOPLEVEL = 1 << 0,

    /** Indicates that #FJ_APP_LOOP can be called multiple times, even within
        itself to wait for events at arbitrary times. */
    FJ_APP_LOOP_FLAG_REENTRANT = 1 << 1,

    /** Indicates that the #FJ_APP_STOP_LOOP message is supported.

        \note
        - **UIKit**: the loop is not stoppable. The only way to forcefully exit
            the program is to call the exit function, which is
            [considered a bad practice by Apple
            ](https://developer.apple.com/library/archive/qa/qa1561/_index.html).
        */
    FJ_APP_LOOP_FLAG_STOPPABLE = 1 << 2,

    /** Indicates that stopping the loop will result into some visible
        consequences, potencially looking like glitches.
        If not present, means that stopping the loop is always done gracefully
        and with no possible harm to the app's visual state.

        To avoid problems connected with this flag, you should minize your
        usage of event loops and instead structure your code so that it just
        responds to messages.

        \note
        - **WinAPI**: if the loop is stopped while a window is being resized
            by the user, the resizing will stop. This is because window resizing
            is implemented on the client side (not by the system) and works by
            [running an internal event loop
            ](https://stackoverflow.com/a/21201822) to track the user's pointer
            position to resize the window.
            All the events still get processed by the library,
            however stopping the loop will cause the resizing to stop.
            */
    FJ_APP_LOOP_FLAG_STOP_VISIBLE = 1 << 3,
};

/// \END


/// \BEGIN{app_messages}

enum fj_app_message
{
    /** Initializes the application.
        \param[in] extra_init_data Points to extra platform-defined
            initialization data (set to NULL to ignore). */
    FJ_APP_INIT,

    /** All created objects should be destroyed before destroying the app. */
    FJ_APP_DEINIT,

    /** Runs the message loop awaiting the #FJ_APP_STOP_LOOP.
        The support for this message and its exact behavior is
        platform-defined. To get a detailed information about that, use
        #FJ_APP_GET_LOOP_FLAGS. */
    FJ_APP_LOOP,

    FJ_APP_DID_START_LOOP,

    /** Indicates that the most recently started loop should stop as soon as
        possible.

        Stopping the loop may not stop the event processing immediately as it
        is often done in batches using queues. Therefore some events can still
        be processed before stopping. */
    FJ_APP_STOP_LOOP,

    /** This is sent right before returning from the running loop. */
    FJ_APP_DID_STOP_LOOP,

    /** Gets the loop capability flags in case #FJ_APP_LOOP is supported.
        If this message is unsupported, that means that #FJ_APP_LOOP is
        unsupported, too.

        \param[out] flags Returns #fj_app_loop_flags. */
    FJ_APP_GET_LOOP_FLAGS,

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

        \param[in] system_handle Platform-defined. */
    FJ_APP_DID_SET_SYSTEM_HANDLE,

    FJ_APP_MESSAGE_MAX,
    FJ_APP_MESSAGE_ENSURE_INT32 = INT32_MAX,
};


/// \END


#endif
