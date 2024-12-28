#ifndef FEJIX_INTERFACE_CLIENT_H_
#define FEJIX_INTERFACE_CLIENT_H_


#include <fejix/core/base.h>


typedef uint32_t fj_client_implementation_id_t;

enum fj_client_id {
    /** Android Native Development Kit */
    FJ_CLIENT_IMPLEMENTATION_ANDK,
    /** Apple Cocoa */
    FJ_CLIENT_IMPLEMENTATION_COCOA,
    /** Wayland protocol */
    FJ_CLIENT_IMPLEMENTATION_WAYLAND,
    /** Windows API */
    FJ_CLIENT_IMPLEMENTATION_WINAPI,
    /** X11 protocol */
    FJ_CLIENT_IMPLEMENTATION_X11,

    FJ_CLIENT_IMPLEMENTATION_MAX = FJ_CLIENT_IMPLEMENTATION_X11,
};


enum fj_client_request_id {
    FJ_CLIENT_GET_IMPLEMENTATION_ID = FJ_REQUEST_PUBLIC_STATIC,
    FJ_CLIENT_GET_IMPLEMENTATION_VERSION,

    FJ_CLIENT_GET_WINDOW_MANAGER = FJ_REQUEST_PUBLIC_STATIC_SUBINTERFACING,
    FJ_CLIENT_GET_SOFTER_MANAGER,
    FJ_CLIENT_GET_OPENGL_MANAGER,

    FJ_CLIENT_ALLOC = FJ_REQUEST_PUBLIC_INITIALISATION,
    FJ_CLIENT_DEALLOC,
    FJ_CLIENT_INIT,
    FJ_CLIENT_DEINIT,
    FJ_CLIENT_INIT_NAME,

    FJ_CLIENT_LAUNCH = FJ_REQUEST_PUBLIC,
    FJ_CLIENT_IDLE,

    /**
        Accepts an optional request data which specifies the timeout.
        May be overridden by FORCED_SLEEP and FORCED_HIBERNATE.
    */
    FJ_CLIENT_SCHEDULE_WAKEUP,

    /** Does nothing on platforms that do not support it. */
    FJ_CLIENT_WAKEUP_INSTANTLY,

    /** Does nothing on platforms that do not support it. */
    FJ_CLIENT_SET_FINISHED,

    /** Does nothing on platforms that do not support it. */
    FJ_CLIENT_BLOCK_FOR_EVENTS,

    /**
        The client looses input focus, so the next sleep may not return in a while.
        Analogous to UIApplicationDelegate/applicationWillResignActive.
    */
    FJ_CLIENT_FORCED_SLEEP,

    /**
        The client entered background.
        Analogous to UIApplicationDelegate/applicationDidEnterBackground.
    */
    FJ_CLIENT_FORCED_HIBERNATE,
};


FJ_DECLARE_ABSTRACT_OBJECT(fj_client)

struct fj_client_schedule_wakeup_info {
    /**
        Can be 0 (wakes up instantly), positive, or infinite (sleeping until any events are
        received).

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
    fj_seconds_t timeout;
};


/** Returns NULL if the specified implementation was not built into the library. */
FJ_PUBLIC
fj_responder_t fj_client_get_builtin_implementation(fj_client_implementation_id_t id);

FJ_PUBLIC
fj_err_t fj_client_get_default_implementation_id(fj_client_implementation_id_t *out_id);


#endif
