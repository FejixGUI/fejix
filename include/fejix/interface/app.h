#ifndef FEJIX_INTERFACE_APP_H_
#define FEJIX_INTERFACE_APP_H_


#include <fejix/core/base.h>


typedef uint32_t fj_app_implementation_id_t;

enum fj_app_implementation_id {
    /** Android Native Development Kit */
    FJ_APP_IMPLEMENTATION_ANDK,
    /** Apple Cocoa */
    FJ_APP_IMPLEMENTATION_COCOA,
    /** Wayland protocol */
    FJ_APP_IMPLEMENTATION_WAYLAND,
    /** Windows API */
    FJ_APP_IMPLEMENTATION_WINAPI,
    /** X11 protocol */
    FJ_APP_IMPLEMENTATION_X11,

    FJ_APP_IMPLEMENTATION_MAX = FJ_APP_IMPLEMENTATION_X11,
};


typedef uint32_t fj_app_extension_id_t;

enum fj_app_extension_id {
    FJ_APP_EXTENSION_APP_MANUAL_SLEEP,
    FJ_APP_EXTENSION_WINDOW_MANAGER,
    FJ_APP_EXTENSION_OPENGL_MANAGER,
    FJ_APP_EXTENSION_RAM_MANAGER,
};


typedef uint32_t fj_app_force_command_t;

enum fj_app_force_command {
    /** The app should finish because of e.g. its own request, a system shutdown etc. */
    FJ_APP_FORCE_FINISH,

    /**
        The app lost primary focus due to interruption by another app.
        This may also be referred to as "deactivated" state.

        The app may not wake up from sleep until it gains focus again.
    */
    FJ_APP_FORCE_SLEEP,

    /**
        The app was moved to the background and should end its current tasks or start background
        tasks.

        The app may not wake up until it moves to the foreground.
    */
    FJ_APP_FORCE_HIBERNATE,


    /** The app should finish due to low system memory. */
    FJ_APP_FORCE_DEALLOCATE,
};


FJ_DEFINE_TAGGED_STRUCT(fj_app)


struct fj_app_create_info {
    union fj_tag tag;
    char const *name;
};


struct fj_app_callbacks {
    fj_err_t (*on_idle)(struct fj_app *app);

    fj_err_t (*on_force)(struct fj_app *app, fj_app_force_command_t command);
};


struct fj_app_funcs {
    fj_app_implementation_id_t (*get_implementation_id)(void);

    void (*get_implementation_version)(struct fj_version *out_version);

    void const *(*get_extension)(fj_app_extension_id_t id);

    fj_err_t (*create)(
        struct fj_app **out_app,
        struct fj_app_create_info const *info,
        struct fj_app_callbacks const *callbacks
    );

    fj_err_t (*destroy)(struct fj_app *app);

    /**
        This function has a lot of implementation-specific behaviors:
        * it can run the whole app lifecycle
        * it can return immediately expecting some external process to run the app
        * it can ignore when the app marks itself as finished, waiting for the system to decide so
        * it can possibly never return upon app termination

        To handle the program lifecycle in the most cross-platform way, the program entrypoint,
        from which this function is called, should end on the call. App deinitialisation should
        happen in response to force commands.
    */
    fj_err_t (*launch)(struct fj_app *app);

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
    void (*wakeup_after_timeout)(struct fj_app *app, fj_seconds_t timeout);

    /**
        Immediately wakes up a sleeping app from another thread.

        Implementation-defined behavior: after calling this on the main thread, the next time when
        trying to sleep the app can immediately wake up, thus clearing the wakeup timeout.
        To handle this, the app should always set a relevant wakeup timeout when idling.
    */
    fj_err_t (*wakeup_immediately)(struct fj_app *app);

    /**
        Marks the app as finished.

        Depending on the implementation, this may force the app to finish.
        Even if the app will finish, it may still process some events before it actually finishes.

        To handle the program termination in the most cross-platform way, the program should do its
        deinitialisation in response to force commands.
    */
    void (*set_finished)(struct fj_app *app);
};


/** Returns NULL for unknown IDs. */
FJ_EXPORT
char const *fj_app_get_implementation_name(fj_app_implementation_id_t id);

/** Returns NULL if the specified implementation was not built into the library. */
FJ_EXPORT
struct fj_app_funcs const *fj_app_get_builtin_implementation(fj_app_implementation_id_t id);

FJ_EXPORT
fj_err_t fj_app_get_default_implementation_id(fj_app_implementation_id_t *out_id);


#endif
