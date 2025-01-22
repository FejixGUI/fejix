#ifndef FEJIX_IMPLEMENTATION_H_
#define FEJIX_IMPLEMENTATION_H_


#include <fejix/core/base.h>


typedef uint32_t fj_implementation_id_t;

enum fj_implementation_id {
    /** X11 protocol */
    FJ_IMPLEMENTATION_X11,
    /** Wayland protocol */
    FJ_IMPLEMENTATION_WAYLAND,
    /** Android Native Development Kit */
    FJ_IMPLEMENTATION_ANDK,
    /** Apple Cocoa */
    FJ_IMPLEMENTATION_COCOA,
    /** Apple UIKit */
    FJ_IMPLEMENTATION_UIKIT,
    /** Windows API */
    FJ_IMPLEMENTATION_WINAPI,
    /** Universal Windows Platform */
    FJ_IMPLEMENTATION_UWP,

    FJ_IMPLEMENTATION_PREDEFINED_COUNT,

    FJ_IMPLEMENTATION_USER = 0x1000,
};


typedef uint32_t fj_interface_id_t;

enum fj_interface_id {
    FJ_INTERFACE_APP,
    FJ_INTERFACE_APP_MANUAL_SLEEP,
    FJ_INTERFACE_APP_ACTIVITY_HINTS,
    FJ_INTERFACE_IMAGE_CONSUMER,
    FJ_INTERFACE_SCENE,
    FJ_INTERFACE_SCENE_LAYOUT,
    FJ_INTERFACE_SCENE_LAYOUT_TRIGGER,
    FJ_INTERFACE_SCENE_SYNC,
    FJ_INTERFACE_RAM,
    FJ_INTERFACE_OPENGL,
    FJ_INTERFACE_VULKAN,

    FJ_INTERFACE_PREDEFINED_COUNT,
    FJ_INTERFACE_USER = 0x1000,
};


struct fj_implementation {
    fj_implementation_id_t id;
    struct fj_version version;

    void const *(*get_interface_funcs)(fj_interface_id_t interface_id);
};


/** Returns NULL for unknown IDs. */
FJ_PUBLIC
char const *fj_implementation_get_name(fj_implementation_id_t id);

/** Returns NULL if the specified implementation was not built into the library. */
FJ_PUBLIC
struct fj_implementation const *fj_implementation_get_builtin(fj_implementation_id_t id);

FJ_PUBLIC
fj_err_t fj_implementation_get_default_id(fj_implementation_id_t *out_id);


#endif
