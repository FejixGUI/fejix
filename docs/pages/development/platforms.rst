=============
Platforms
=============



Platform names
----------------

Use full platform names as-is in documentation/comments.

Use full platform names in lowercase everywhere in code, except for namespacing
C macros, types, and functions.

The C "namespace" is shortened to be max. 6 characters long.

..  list-table::
    :header-rows: 1

    *   -   Platform
        -   C namespace
    *   -   Android NDK
        -   ``andk``
    *   -   BSD Framebuffer
        -   ``bsdfb``
    *   -   Cocoa
        -   ``cocoa``
    *   -   Linux DRM
        -   ``lxdrm``
    *   -   Linux Framebuffer
        -   ``lxfb``
    *   -   UIKit
        -   ``uikit``
    *   -   WebAssembly
        -   ``wasm``
    *   -   Windows API
        -   ``winapi``
    *   -   Wayland
        -   ``wl``
    *   -   X11
        -   ``x11``

Full names
''''''''''''''''

Do:

* ``FJ_OPT_WAYLAND``
* ``#include <src/wayland/something.h>``

Don't:

* ``FJ_OPT_WL``
* ``#include <src/wl/something.h>``

C namespaces
'''''''''''''''''''''

Do:

* ``fj_wl_create_window()``
* ``struct fj_wl_window_data { ... };``

Don't:

* ``fj_wayland_create_window()``
* ``struct fj_wayland_window_data { ... };``

Check your understanding
'''''''''''''''''''''''''

.. code-block:: c

    char const *fj_wl_get_platform_name(void)
    {
        return "wayland";
    }
