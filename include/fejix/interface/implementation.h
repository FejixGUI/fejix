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
    FJ_INTERFACE_LAYER,
    FJ_INTERFACE_CANVAS,
    FJ_INTERFACE_BLANK,
};


struct fj_implementation_iface {
    void const * (* get)(fj_interface_id_t iface_id);

    fj_implementation_id_t id;
    fj_version_t version;
};


#endif
