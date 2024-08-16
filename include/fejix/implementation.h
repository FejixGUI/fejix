#ifndef FEJIX_IMPLEMENTATION_H_
#define FEJIX_IMPLEMENTATION_H_


#include <fejix/core/base.h>


typedef uint32_t fj_implementation_id_t;

enum fj_implementation_id {
    /** Android Native Development Kit */
    FJ_IMPLEMENTATION_ANDK,
    /** Apple AppKit */
    FJ_IMPLEMENTATION_APPKIT,
    /** Wayland protocol */
    FJ_IMPLEMENTATION_WAYLAND,
    /** Windows API */
    FJ_IMPLEMENTATION_WINAPI,
    /** X11 protocol */
    FJ_IMPLEMENTATION_X11,

    FJ_IMPLEMENTATION_MAX,
};

typedef uint32_t fj_interface_id_t;

enum fj_interface_id {
    FJ_INTERFACE_CLIENT,
    FJ_INTERFACE_WM,
    FJ_INTERFACE_CANVAS,
};


struct fj_implementation {
    fj_implementation_id_t id;
    fj_version_t version;
    struct fj_client_iface const */*?*/ client;
    struct fj_wm_iface const */*?*/ wm;
};


/** The returned array is sorted by implementation IDs. */
void fj_get_builtin_implementations(
    struct fj_implementation const *const */*[]? out*/ * implementations,
    uint32_t /*out*/ * implementation_count
);

/** Returns the implementation name corresponding to the given ID.
    Returns NULL for non-existing IDs */
char const */*[]?*/ fj_get_implementation_name(
    fj_implementation_id_t implementation_id
);


#endif
