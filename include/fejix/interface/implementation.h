#ifndef FEJIX_INTERFACE_IMPLEMENTATION_H_
#define FEJIX_INTERFACE_IMPLEMENTATION_H_


#include <fejix/core/base.h>


FJ_EXTERN_C_BEGIN


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
    FJ_INTERFACE_SCHEDULER,
    FJ_INTERFACE_OUTPUT,
    FJ_INTERFACE_SOFTER_CANVAS,
};


struct fj_implementation_iface {
    fj_implementation_id_t id;
    fj_version_t version;

    void const *(*get_interface)(fj_interface_id_t interface_id);
};


/**
:param implementations: Returns a nullable array of implementation pointers, sorted by
implementation ID.
:param implementation_count: Returns the implementation count.
*/
void fj_get_builtin_implementations(
    struct fj_implementation_iface const *const **implementations,
    uint32_t *implementation_count
);


FJ_EXTERN_C_END

#endif
