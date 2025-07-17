===================
Development
===================


Platforms
===============

Platform names
----------------

Use full platform names as-is in documentation/comments.

Use full platform names in lowercase everywhere in code, except for namespacing
C macros, types, and functions.

The C "namespace" is shortened to be max. 6 characters long.

.. list-table::
  :header-rows: 1

  * - Platform
    - C namespace
    - Description
  * - Android NDK
    - ``andk``
    - Android Native Development Kit.
  * - BSD Framebuffer
    - ``bsdfb``
    - Used in the BSD family of operating systems without a graphical
      environment for rendering with no hardware acceleration.
  * - Cocoa
    - ``cocoa``
    - Apple Cocoa, used in macOS.
  * - Linux DRM
    - ``lxdrm``
    - Used in Linux without a graphical environment for rendering with
      hardware acceleration.
  * - Linux Framebuffer
    - ``lxfb``
    - Used in Linux without a graphical environment for rendering with no
      hardware acceleration.
  * - UIKit
    - ``uikit``
    - Apple UIKit, a modern successor to Cocoa, used in all Apple operating
      systems.
  * - WebAssembly
    - ``wasm``
    - Used in browsers.
  * - Windows API
    - ``winapi``
    - Used in Microsoft Windows desktop OS.
  * - Wayland
    - ``wl``
    - A modern replacement for X11.
  * - X11
    - ``x11``
    - The X Window System, version 11, used in Unix-like operating systems.

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

  struct fj_platform fj_wl_platform {
      .name = "wayland"
  };

Documentation
=================

The project documentation is built using:

* `Doxygen <https://www.doxygen.nl/>`_
* `Sphinx <https://www.sphinx-doc.org/en/master/>`_
* `Breathe <https://breathe.readthedocs.io/en/latest/index.html>`_
* `Furo <https://pradyunsg.me/furo/>`_

Setup
------------

#. Install Doxygen.

#. `Create <https://docs.python.org/3/library/venv.html#creating-virtual-environments>`_
   and `activate <https://docs.python.org/3/library/venv.html#how-venvs-work>`_
   a Python virtual environment:

   .. code-block:: shell

     python3 -m venv .venv
     source .venv/bin/activate  # or activate.fish, or activate.csh

   The activation script path depends on the OS and the Python version.

Build
------------

Run ``Make`` from the ``docs`` directory:

.. code-block:: shell

  cd docs && make && make preview

or:

.. code-block:: shell

  make -C docs && make preview -C docs

``make preview`` opens ``build/docs/html/index.html``.

Markup
------------

.. TODO give an overview of how the docs are structured

.. tip::
  Write ``\---`` instead of ``---`` for mdashes in Doxygen comments
  (same applies for ndashes, ``\--``).
  As Doxygen output gets further processed by Sphinx, apparently XML entities
  in the intermediate output get escaped.
  This looks like this:

  .. list-table::

    * - Doc comment
      - Doxygen output (XML)
      - Sphinx output (HTML)
    * - ``---``
      - ``&8212;``
      - &8212;
    * - ``\---``
      - ``---``
      - ---


Coding style
===============

TODO, FIXME, XXX
----------------------------

Mark the important things with the following set of markers:

* For a work in progress use ``TODO``:

  In ordinary C comments:

  .. code-block:: c

    // TODO <message>

  In Doxygen comments:

  .. code-block:: c

    /** \noop TODO <message> */

  In RST documentation:

  .. code-block:: rst

    .. TODO <message>

* For bugs that need to be fixed use ``FIXME`` near the code they appear:

  In C comments:

  .. code-block:: c

    // FIXME <message>

  In RST documentation:

  .. code-block:: rst

    .. FIXME <message>

* For important implementation notes use ``XXX`` near the code that relies on
  or works around some badly documented or surprising platform features:

  .. code-block:: c

    // XXX <message>

This makes it easy to search for places in code that require attention with
``ripgrep``:

.. code-block:: shell

  rg 'TODO|FIXME|XXX'

Only use ``\note``, ``\warning``, ``\bug``, or similar tags in public
documentation for things that users need to know about.
Do not use these tags in private code.


