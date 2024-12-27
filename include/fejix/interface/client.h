#ifndef FEJIX_INTERFACE_CLIENT_H_
#define FEJIX_INTERFACE_CLIENT_H_


#include <fejix/core/base.h>


enum fj_client_request_id {
    FJ_CLIENT_ALLOC,
    FJ_CLIENT_DEALLOC,
    FJ_CLIENT_INIT,
    FJ_CLIENT_INIT_NAME,
    FJ_CLIENT_DEINIT,

    FJ_CLIENT_LAUNCH,
    FJ_CLIENT_QUIT,
    FJ_CLIENT_IDLE,
    FJ_CLIENT_SET_SLEEP_TIMEOUT,
    FJ_CLIENT_BLOCK_FOR_EVENTS,
    FJ_CLIENT_WAKEUP,

    /** Received on launching. */
    FJ_CLIENT_FORCED_ETERNAL,

    /** Received on launching. */
    FJ_CLIENT_FORCED_NONBLOCKING,

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


#endif
