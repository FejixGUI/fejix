====================
<fejix/platform.h>
====================

Platform loading
================================================================================

..  function:: void fj_platform_get_builtin_list(char const *const **out_platforms, uint32_t *out_platforms_length)

    :param out_platforms: Returns NULL if there are no builtin platforms.
    :param out_platforms_length: Returns the number of builtin platforms.

..  function:: char const *fj_platform_load(void)

    Automatically deduces and loads the preferred platform.

    :returns:
        Tries the following cases, in the following order:

        #.  If there are no available platforms, then NULL.
        #.  If there is only one platform, then that platform.
        #.  If ``FEJIX_PLATFORM`` environment variable is defined and such
            a platform is present, then that.
        #.  Tries to deduce from other environment variables, in particular:

            -   Wayland and X11 are chosen based on ``XDG_SESSION_TYPE``.

        #.  If all of the above fails, returns the first platform on the list.


Platform functions
================================================================================


..  var:: char const *(*fj_platform_get_name)(void)

    :returns:
        The full name of the platform in lowercase without spaces, e.g.
        ``windowsapi``, ``wayland`` etc. (see :ref:`platforms`).

..  var:: fj_version (*fj_platform_get_version)(void)

