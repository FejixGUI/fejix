/** \HEADER */

#ifndef FEJIX_PLATFORM_H_
#define FEJIX_PLATFORM_H_


#include <fejix/base.h>


/// \BEGIN{platform_definition}


struct fj_platform
{
    char const *name;
    struct fj_version version;

    /** \param dispatcher_type Identifies what object class to return
            the dispatcher for.
        \returns NULL if the module of the dispatcher is not supported. */
    fj_dispatcher (*get_dispatcher)(enum fj_dispatcher_type dispatcher_type);
};

/// \END

/// \BEGIN{platform_loading}

/** `out_platforms` returns NULL if there are no builtin platforms. */
FJ_PUBLIC
void fj_platform_get_builtin_list(
    struct fj_platform const *const **out_platforms,
    uint32_t *out_platforms_length);

/** Automatically deduces and loads the preferred platform.

    \returns Tries the following cases, in the following order:
    1. If there are no available platforms, then NULL.
    2. If there is only one platform, then that platform.
    3. If `FEJIX_PLATFORM` environment variable is defined and such a platform
        is present, then that.
    4. Tries to deduce from other environment variables, in particular:
        - Wayland and X11 are chosen based on `XDG_SESSION_TYPE`.
    5. If all of the above fails, returns the first platform on the list.
*/
FJ_PUBLIC
struct fj_platform const *fj_platform_load(void);
// TODO Add functions to modify the behavior of the loader, e.g.
// allowed/disallowed platforms etc.

/// \END


#endif
