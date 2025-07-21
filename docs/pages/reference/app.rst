================================================================================
<fejix/app.h>
================================================================================

App
================================================================================

..  struct:: fj_app

    This object manages the application lifecycle and all of its resources.

    ..  note::
        This should generally be a instantiated once per program.
        Whether it can be instantiated multiple times is implementation-defined.

    *   **Windows API**, **Wayland**, **X11**: an application object can be
        instantiated as many times as you want.

    ..  note::
        This should generally be instantiated on the main thread.
        Whether it can be instantiated on other threads is platform-defined.

    *   **UIKit**: this *must* be instantiated on the main thread and all
        messsages where thread-safety is not specified *must* be sent from
        the main thread.
    *   **Windows API**, **Wayland**, **X11**: this can be instantiated on any
        thread.

    ..  note::
        This should generally be run on the same thread where instantiated.
        Whether or not it can be run on other threads is platform-defined.

    *   **Wayland**, **X11**: this can be run from any thread.
    *   **Windows API**: there are restrictions that limit event handling to the
        thread that has created all the objects that may receive events.
        This is because `event queues are thread-local <https://learn.microsoft.com/en-us/windows/win32/winmsg/using-messages-and-message-queues>`_
        and windows are `bound to thread event queues at creation
        <https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-peekmessagew>`_.
        Because of this, it is better to create the app and all other objects
        on the same thread that handles them.


Methods
--------------------------------------------------------------------------------


..  var:: fj_err (*fj_app_create)(struct fj_app **out_app, void *user_data);

    Initializes the application.

    :param out_app:
        Pointer to a pointer to the application structure.
        The function will allocate the structure and set the pointer.
        To check for errors, never check the returned pointer for NULL,
        handle the return value.

    :param user_data: Optional.


..  var:: fj_err (*fj_app_destroy)(struct fj_app *app);

    Deinitializes the application.

    This message is not intended for quitting the app as the quitting
    process can be complicated and on some platforms even asynchronous.
    Use :func:`fj_app_quit` instead.

    All created objects should be destroyed before destroying the app.




..  var:: fj_err (*fj_app_run)(struct fj_app *app);
..  var:: fj_err (*fj_app_iterate)(struct fj_app *app);
..  var:: fj_err (*fj_app_await)(struct fj_app *app);
..  var:: fj_err (*fj_app_quit)(struct fj_app *app);

..  var:: fj_err (*fj_app_ping)(struct fj_app *app);

    Wakes up an application that is waiting for events.
    This asks the system to send a custom event that goes back as
    :func:`fj_app_ping_cb`.

    This can be sent from another thread provided that the app is not
    being destroyed.

    This is not supposed to be called periodically from the main thread
    as this goes through an inefficient process of communicating with the
    system. To regularly invoke a user callback on the main thread,
    use a timer with zero timeout period.


Callbacks
--------------------------------------------------------------------------------


..  var:: fj_err (*fj_app_set_run_modes_cb)(struct fj_app *app, enum fj_app_run_modes const modes);
..  var:: fj_err (*fj_app_iterate_cb)(struct fj_app *app);
..  var:: fj_err (*fj_app_quit_cb)(struct fj_app *app);
..  var:: fj_err (*fj_app_ping_cb)(struct fj_app *app);
..  var:: fj_err (*fj_app_activate_cb)(struct fj_app *app);
..  var:: fj_err (*fj_app_deactivate_cb)(struct fj_app *app);
..  var:: fj_err (*fj_app_suspend_cb)(struct fj_app *app);
..  var:: fj_err (*fj_app_resume_cb)(struct fj_app *app);

..  var:: fj_err (*fj_app_set_system_handle_cb)(struct fj_app *app, uintptr_t handle);

    Provides an internal system handle of the app.
    This is sent on app initialization.

    ..  note::
        *   **X11**: the handle is the Xlib's :expr:`Display*`.
        *   **Wayland**: the handle is :expr:`wl_display*`.
        *   **Windows API**: the handle is the current :expr:`HINSTANCE`.


Utils
--------------------------------------------------------------------------------

..  enum:: fj_app_run_modes

    ..  enumerator:: FJ_APP_RUN_MODE_RUNNABLE   = 1<<0
    ..  enumerator:: FJ_APP_RUN_MODE_QUITTABLE  = 1<<1
    ..  enumerator:: FJ_APP_RUN_MODE_ITERABLE   = 1<<2
    ..  enumerator:: FJ_APP_RUN_MODE_AWAITABLE  = 1<<3

