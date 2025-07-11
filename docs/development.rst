===================
Development
===================


Platforms
===============

Platform names
----------------

The codename of the platform is the C "namespace", the value of the
:c:member:`fj_platform.name` field, and used everywhere throughout the codebase.

The codename should be max. 6 characters long.

.. list-table::
  :header-rows: 1

  * - Full name
    - Codename
  * - Windows API
    - ``winapi``
  * - Windows Runtime
    - ``winrt``
  * - X11
    - ``x11``
  * - Wayland
    - ``wl``
  * - Apple Cocoa
    - ``cocoa``
  * - Apple UIKit
    - ``uikit``
  * - Android Native Development Kit
    - ``andk``
  * - WebAssembly
    - ``wasm``
  * - Linux Framebuffer
    - ``lxfb``
  * - Linux Direct Rendering Manager
    - ``lxdrm``
  * - BSD Framebuffer
    - ``bsdfb``

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

  cd docs && make
  # -or- (shorter)
  make -C docs

To preview the docs in your browser you can run:

.. code-block:: shell

  make -C docs preview

this opens ``build/docs/html/index.html``.

Markup
------------

.. TODO give an overview of how the docs are structured

.. tip::
  Use the project's Doxygen command aliases:

  * ``\HEADER``

  instead of ``\file`` because it additionally generates a short include
  statement in the header description, and

  * ``\BEGIN{GROUP_NAME}``
  * ``\BEGIN{GROUP_NAME,GROUP_TITLE}``
  * ``\END``

  instead of repeating ``\addtogroup`` and ``\{`` on separate lines.
  To add a group description, just put a doc comment right above the ``\BEGIN``
  command.


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


