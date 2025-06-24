# Fejix

Fejix is a cross-platform GUI base library
— it wraps system GUI APIs (Windows API, Wayland, X11, Cocoa, etc.)
required to build UI frameworks, game engines or other tools.

Fejix is designed to be a complete GUI system abstraction, resusable by all kinds of cross-platform
applications, yet simple enough to extend, port or even completely reimplement.

Written in C99.

**Features:**

- **Full range of GUI capabilities**:
    window management, input handling, system notifications, clipboard, notifier icons, etc.
    We would be happy to support as much useful features as reasonably possible.
- **Compatibility**:
    we implement some legacy or platform-specific features to allow existing projects
    (think SDL or Qt) to potencially migrate to Fejix without breaking their code (too much).
- **Portable backend system**:
    backends implement the Fejix API directly, but (almost) every function is optional.
    For unimplemented functions Fejix provides a default no-op fallback.
    This makes it extrenely simple to add new backends as all you need is to create a file
    and add it in CMake files. No complicated initialisation logic or dynamic object-oriented
    magic.

> This project is in its earliest development stage.
>
> The most interesting thing to see here is this emoji: 🌸.
>
> Yay.


## License

Copyright © 2024-2025 Mark Lagodych.
Licensed under [Apache-2.0](./LICENSE.txt)
