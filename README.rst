=========
Fejix
=========

Fejix is a cross-platform GUI base library.
It can be used to build UI frameworks, game engines and other tools that need to use system APIs
for things like windows, mouse/keyboard/touch input, graphical API interop etc.

Fejix is *not* a graphics rendering engine or a full-blown GUI framework.

Goals:

* **feature-rich**:

    No extension libraries.
    Functionality is either in Fejix or does not exist.

* **no bloat**:

    No object-oriented metaprogramming like in GDK or Qt Base.
    Just simple enough C.

* **modularity**:

    Every bit of functionality is used though its own **module**
    which may or may not be available at runtime.
    Simple to design, extend, port, and reimplement.

::

    Note, however, that hardly anything has been implemented.

    This library is in its earliest development stage.

    The most interesting thing to see here is this emoji: 🌸.

    Yay.


License
=========

Copyright 2024-2025 Mark Lagodych.
Licensed under `Apache-2.0`_

.. _`Apache-2.0`: ./LICENSE.txt.
