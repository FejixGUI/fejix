#ifndef FEJIX_APP_INCLUDED
#define FEJIX_APP_INCLUDED


#include <fejix/core/primitives.h>


typedef uint32_t fj_app_suspend_type_t;

enum fj_app_suspend_type {
    /**
        The app lost primary focus due to interruption by another app.
        This may also be referred to as "deactivated" state.

        The app may not wake up from sleep until it gains focus again.
    */
    FJ_APP_SUSPEND_PAUSE,

    /**
        The app was moved to the background and should end its current tasks or start background
        tasks.

        The app may not wake up until it moves to the foreground.
    */
    FJ_APP_SUSPEND_HIBERNATE,
};


typedef uint32_t fj_app_finish_type_t;

enum fj_app_finish_type {
    /** Finish due to the app's own request. */
    FJ_APP_FINISH_NORMAL,

    /** System shutdown. The app should finish as soon as possible. */
    FJ_APP_FINISH_SHUTDOWN,

    /** The app should finish if possible in order to free up memory. */
    FJ_APP_FINISH_LOW_MEMORY,
};


struct fj_app;


/** Returns ``FJ_VERSION(0,0,0)`` (equal to ``0``) if unsupported. */
FJ_PUBLIC
fj_version_t fj_app_get_interface_version(void);

FJ_PUBLIC
fj_err_t fj_app_create(struct fj_app **out_app, void *app_userdata);

FJ_PUBLIC
fj_err_t fj_app_destroy(struct fj_app *app);


typedef fj_err_t (*fj_app_on_continue_fn_t)(void *app_userdata);

FJ_PUBLIC
void fj_app_on_continue(struct fj_app *app, fj_app_on_continue_fn_t callback);


typedef fj_err_t (*fj_app_on_suspend_fn_t)(void *app_userdata, fj_app_suspend_type_t type);

FJ_PUBLIC
void f_app_on_suspend(struct fj_app *app, fj_app_on_suspend_fn_t callback);


typedef fj_err_t (*fj_app_on_finish_fn_t)(void *app_userdata, fj_app_finish_type_t type);

FJ_PUBLIC
void fj_app_on_finish(struct fj_app *app, fj_app_on_finish_fn_t callback);


/**
    This function has a lot of implementation-specific behaviors:
    * it can run the whole app lifecycle
    * it can return immediately expecting some external process to run the app
    * it can ignore when the app marks itself as finished, waiting for the system to decide so
    * it can possibly never return upon app termination

    To handle the program lifecycle in the most cross-platform way, the program entrypoint,
    from which this function is called, should end on the call. App deinitialisation should
    happen in the finish callback.
*/
FJ_PUBLIC
fj_err_t fj_app_launch(struct fj_app *app);


/**
    If called from the main thread, makes the app continue immediately after processing events
    without waiting.

    If called from another thread, makes the app wake up from sleeping while waiting for events.
*/
FJ_PUBLIC
fj_err_t fj_app_request_continue(struct fj_app *app);


/**
    Marks the app as finished.

    Depending on the implementation, this may force the app to finish.
    Even if the app will finish, it may still process some events before it actually finishes.

    To handle the program termination in the most cross-platform way, the program should do its
    deinitialisation in response to the finish callback.
*/
FJ_PUBLIC
fj_err_t fj_app_request_finish(struct fj_app *app);


#endif
