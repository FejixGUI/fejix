#ifndef FEJIX_APP_H_
#define FEJIX_APP_H_


#include <fejix/core/primitives.h>


typedef uint32_t fj_app_notification_t;

enum fj_app_notification {
    /**
        The app must terminate because of e.g. its own request, a system shutdown etc.

        Not responding to this command in sane time (e.g. a few seconds) may lead to the operating
        system terminating the process.
    */
    FJ_APP_NOTIFICATION_TERMINATE,

    /** The app should finish due to low system memory. */
    FJ_APP_NOTIFICATION_DEALLOCATE,

    /**
        The app lost primary focus due to interruption by another app.
        This may also be referred to as "deactivated" state.

        The app may not wake up from sleep until it gains focus again.
    */
    FJ_APP_NOTIFICATION_SLEEP,

    /**
        The app was moved to the background and should end its current tasks or start background
        tasks.

        The app may not wake up until it moves to the foreground.
    */
    FJ_APP_NOTIFICATION_HIBERNATE,
};


struct fj_app;


/** Called before the app goes to sleep waiting for events. */
typedef fj_err_t (*fj_app_on_idle_fn_t)(struct fj_app *app);

typedef fj_err_t (*fj_app_on_finish_fn_t)(struct fj_app *app);

typedef fj_err_t (*fj_app_on_notification_fn_t)(
    struct fj_app *app, fj_app_notification_t notification);


struct fj_app_create_info {
    void *userdata;
};


FJ_PUBLIC
fj_err_t fj_app_check_supported(void);

FJ_PUBLIC
fj_err_t fj_app_create(struct fj_app **out_app, struct fj_app_create_info const *info);

FJ_PUBLIC
fj_err_t fj_app_destroy(struct fj_app *app);

FJ_PUBLIC
void fj_app_on_idle(struct fj_app *app, fj_app_on_idle_fn_t callback);

FJ_PUBLIC
void fj_app_on_finish(struct fj_app *app, fj_app_on_finish_fn_t callback);

FJ_PUBLIC
void fj_app_on_notification(struct fj_app *app, fj_app_on_notification_fn_t callback);


/**
    This function has a lot of implementation-specific behaviors:
    * it can run the whole app lifecycle
    * it can return immediately expecting some external process to run the app
    * it can ignore when the app marks itself as finished, waiting for the system to decide so
    * it can possibly never return upon app termination

    To handle the program lifecycle in the most cross-platform way, the program entrypoint,
    from which this function is called, should end on the call. App deinitialisation should
    happen in ``on_finish()``.
*/
FJ_PUBLIC
fj_err_t fj_app_launch(struct fj_app *app);


/**
    Sets the timeout after which to wake up if no events arrive.

    :param timeout: Can be 0 (wakes up instantly), positive, or infinite (sleeping until any
        events are received).

    The timeout is cleared after every sleep.

    Timeouts in practice are very imprecise. They have system-defined limits (typically
    ``1..2^31`` milliseconds), may get errors due to system task scheduling, can be rounded to
    system clock granularity (e.g. 1/64 of a second on Windows by default) with random
    unpredictable rounding errors, or even completely drift (the bigger the timeout the bigger
    the drift).

    Timeouts may also be disrespected because the system (e.g. on mobile platforms) can bring
    the program to a deactivated state (temporary loss of focus, e.g. when interrupted by
    another program) or to the background (complete loss of focus, e.g. when exited to the
    "Home" screen). In such cases the client may not be able to wake up until the system brings
    the program to its normal state.
*/
FJ_PUBLIC
void fj_app_wakeup_after_timeout(struct fj_app *app, fj_seconds_t timeout);


/**
    Immediately wakes up a sleeping app from another thread.

    Implementation-defined behavior: after calling this on the main thread, the next time when
    trying to sleep the app can immediately wake up, thus clearing the wakeup timeout.
    To handle this, the app should always set a relevant wakeup timeout when idling.
*/
FJ_PUBLIC
fj_err_t fj_app_wakeup_remotely(struct fj_app *app);


/**
    Marks the app as finished.

    Depending on the implementation, this may force the app to finish.
    Even if the app will finish, it may still process some events before it actually finishes.

    To handle the program termination in the most cross-platform way, the program should do its
    deinitialisation in response to the app commands.
*/
FJ_PUBLIC
void fj_app_set_finished(struct fj_app *app);


#endif
