#ifndef FEJIX_IMPLEMENTATION_H_
#define FEJIX_IMPLEMENTATION_H_


#include <fejix/interface/app.h>


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

    FJ_IMPLEMENTATION_MAX = FJ_IMPLEMENTATION_X11,
};


struct fj_implementation {
    fj_implementation_id_t id;
    struct fj_version api_version;
    struct fj_app_class const *app_class;
};


/** Returns NULL for unknown IDs. */
FJ_EXPORT
char const *fj_implementation_get_name(fj_implementation_id_t id);

/** Returns NULL if the specified implementation was not built into the library. */
FJ_EXPORT
struct fj_implementation const *fj_implementation_get_builtin(fj_implementation_id_t id);

FJ_EXPORT
fj_err_t fj_implementation_get_default_id(fj_implementation_id_t *out_id);


#endif
