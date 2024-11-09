#ifndef FEJIX_IMPLEMENTATION_H_
#define FEJIX_IMPLEMENTATION_H_


#include <fejix/core/base.h>


typedef uint32_t fj_implementation_id_t;

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

typedef uint32_t fj_interface_id_t;

enum fj_interface_id {
    FJ_INTERFACE_CLIENT,
    FJ_INTERFACE_SLEEP_TIMER,
    FJ_INTERFACE_WINDOW,
    FJ_INTERFACE_SOFTER,
};


struct fj_implementation {
    fj_implementation_id_t id;
    fj_version_t version;

    void const *(*get_interface_funcs)(fj_interface_id_t interface_id);
};


FJ_EXTERN_C_BEGIN

/**
:param implementations: Returns a nullable array of implementation pointers, sorted by
implementation ID.
:param implementation_count: Returns the implementation count.
*/
void fj_get_builtin_implementations(
    struct fj_implementation const *const **implementations,
    uint32_t *implementation_count
);

FJ_EXTERN_C_END

#endif
