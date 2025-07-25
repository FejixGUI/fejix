/**
    .. _headerdoc:

    Platform
    ============

    ..  autostruct:: fj_platform
        :members:

    ..  autounion:: fj_class
        :members:

    ..  autoenum:: fj_class_type
        :members:

    Platform loading
    ===================

    ..  autofunction:: fj_platform_get_builtin_list
    ..  autofunction:: fj_platform_get_builtin_default
*///

#ifndef FEJIX_PLATFORM_H_
#define FEJIX_PLATFORM_H_


#include <fejix/base.h>


/** */
enum fj_class_type
{
    /** */
    FJ_CLASS_TYPE_APP,
};

struct fj_app_class;

/** */
union fj_class
{
    /** */
    struct fj_app_class const *app_class;
};

/** */
struct fj_platform
{
    /** The name of the platform. */
    char const *name;
    /** The version of the platform. */
    fj_version version;

    /** */
    union fj_class (*get_class)(enum fj_class_type type_id);
};


/**
    Gets the list of builtin platforms.

    :param out_platforms:
        Returns NULL if there are no builtin platforms.
    :param out_platforms_length:
        Returns the number of builtin platforms or 0 if
        there are none.
*/
FJ_PUBLIC
void fj_platform_get_builtin_list(
    struct fj_platform const *const **out_platforms,
    uint32_t *out_platforms_length);


/**
    Deduces the preferred platform out of the available platforms
    built into the library.

    :returns:
        Tries the following cases, in the following order:

        #.  If there are no available platforms, then NULL.
        #.  If there is only one platform, then that platform.
        #.  If ``FEJIX_PLATFORM`` environment variable is defined and such
            a platform is present, then that.
        #.  Tries to deduce from other environment variables, in particular:

            -   Wayland and X11 are chosen based on ``XDG_SESSION_TYPE``.

        #.  If all of the above fails, returns the first platform on the list.
*/
FJ_PUBLIC
struct fj_platform const *fj_platform_get_builtin_default(void);


#endif
