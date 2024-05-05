#ifndef FEJIX_IMPLEMENTATION_H_
#define FEJIX_IMPLEMENTATION_H_


#include <fejix/base.h>


/** The variants are sorted alphabetically except for NOOP. */
enum fj_implementation_id {
    /** No-operation testing implementation that contains no interfaces and is
        basically a dummy.
        This may be useful for some cases when you need to build the library
        without any real implementation. */
    FJ_IMPLEMENTATION_NOOP,
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
};

enum fj_interface_id {
    FJ_INSTANCE,
    FJ_SERVER,
    FJ_RENDER_MANAGER,
    FJ_COMPOSITING_MANAGER,
    FJ_DISPLAY_MANAGER,
};


struct fj_implementation {
    fj_enum32_t implementation_id;

    void *fjOPTION static_data;

    void const *fjOPTION (* get_interface)(
        struct fj_implementation const * implementation,
        fj_enum32_t interface_id
    );
};


/** The returned array is sorted by implementation IDs.
    Implementation count is always greater than 0. */
void fj_get_builtin_implementations(
    uint32_t fjOUT * implementation_count,
    struct fj_implementation const * const *fjARRAY fjOUT * implementations
);


#endif
