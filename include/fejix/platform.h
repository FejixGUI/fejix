/**
    \file
*/

#ifndef FEJIX_PLATFORM_H_
#define FEJIX_PLATFORM_H_


#include <fejix/base.h>


enum fj_dispatcher_type {
    FJ_DISPATCHER_APP,
    FJ_DISPATCHER_WINDOW,
    FJ_DISPATCHER_WINDOW_SERVICE,
};

struct fj_platform {
    char const *name;
    struct fj_version version;

    /**
        \returns The dispatcher function, manually convertible to the appropriate function type.
            NULL if the module of the dispatcher is not supported.
    */
    void (*(*get_dispatcher)(enum fj_dispatcher_type))(void);
};


/** \param[out] out_platforms Returns NULL if there are no builtin platforms. */
FJ_PUBLIC
void fj_platform_get_builtin_list(
    struct fj_platform const *const **out_platforms, uint32_t *out_platforms_length);

/**
    Automatically loads a preferred platform.

    \returns Tries the following cases, whichever succeeds first:
    0. NULL if there are no available platforms.
    1. The only platform if there is only one platform.
    2. The platform specified by `FEJIX_PLATFORM` environment variable
        if it is defined and such a platform is present.
    3. The platform guessed from other environment variables, in particular:
        - Wayland and X11 are chosen based on `XDG_SESSION_TYPE`.
    4. If all of the above fails, returns the first platform on the list.
*/
FJ_PUBLIC
struct fj_platform const *fj_platform_load(void);
// TODO Add functions to modify the behavior of the loader, e.g. allowed/disallowed platforms etc.


#endif
