#ifndef FEJIX_WINDOW_H_
#define FEJIX_WINDOW_H_


#include <fejix/app.h>

// TODO

/**
    This function ensures that all recent changes to the window are sent to the system and
    will come into effect.

    Windows have visible state and because changing parts of the state independently may sometimes
    lead to visual artifacts, some platforms (like Wayland) implement state caching so that it can
    be atomically committed.
*/
// update|commit TODO


#endif
