#ifndef FEJIX_IMPLEMENTATION_H_
#define FEJIX_IMPLEMENTATION_H_


#include <fejix/core/base.h>


enum fj_implementation_id {
    /** Android Native Development Kit */
    FJ_IMPLEMENTATION_ANDK,
    /** Apple Cocoa */
    FJ_IMPLEMENTATION_COCOA,
    /** Wayland protocol */
    FJ_IMPLEMENTATION_WAYLAND,
    /** Windows API */
    FJ_IMPLEMENTATION_WINAPI,
    /** X11 protocol */
    FJ_IMPLEMENTATION_X11,

    FJ_IMPLEMENTATION_MAX,
};

enum fj_interface_id {
    FJ_CLIENT,
    FJ_RENDERING_MANAGER,
    FJ_COMPOSITING_MANAGER,
    FJ_DISPLAY_MANAGER,
};


struct fj_implementation {
    fj_enum32_t implementation_id;
    fj_version_t interface_version;
    struct fj_client const */*?*/ client;
};


/** The returned array is sorted by implementation IDs. */
void fj_get_builtin_implementations(
    struct fj_implementation const *const */*[]? out*/ * implementations,
    uint32_t /*out*/ * implementation_count
);

/** Returns the implementation name corresponding to the given ID.
    Returns NULL for non-existing IDs */
uint8_t const */*[]?*/ fj_get_implementation_name(
    fj_enum32_t implementation_id
);


#endif
