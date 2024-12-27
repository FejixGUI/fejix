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
    FJ_CLIENT_GET_IMPLEMENTATION_ID,
    FJ_CLIENT_GET_IMPLEMENTATION_VERSION,

    FJ_CLIENT_GET_CLIENT_ITERATOR,
    FJ_CLIENT_GET_WINDOW_MANAGER,
    FJ_CLIENT_GET_SOFTER_MANAGER,
    FJ_CLIENT_GET_SOFTER_CANVAS,
    FJ_CLIENT_GET_OPENGL_MANAGER_EGL,
    FJ_CLIENT_GET_OPENGL_MANAGER_GLX,
    FJ_CLIENT_GET_OPENGL_MANAGER_WGL,

    FJ_CLIENT_ALLOC = 0x200,
    FJ_CLIENT_DEALLOC,
    FJ_CLIENT_INIT,
    FJ_CLIENT_INIT_NAME,
    FJ_CLIENT_DEINIT,

    FJ_CLIENT_LAUNCH,
    FJ_CLIENT_IDLE,
    FJ_CLIENT_SET_SLEEP_TIMEOUT,
    FJ_CLIENT_SET_FINISHED,
    FJ_CLIENT_WAKEUP,

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


/** Returns NULL if the specified implementation was not built into the library. */
FJ_PUBLIC
fj_responder_t fj_client_get_builtin_implementation(fj_client_implementation_id_t id);

FJ_PUBLIC
fj_err_t fj_client_get_default_implementation_id(fj_client_implementation_id_t *out_id);


#endif
