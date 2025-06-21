# Fejix

**Fejix** is a cross-platform **GUI base** library
— it wraps system APIs (Windows API, Wayland, X11, Cocoa, etc.)
to provide access to low-level GUI features like
window management, input handling, system notifications, clipboard, notifier icons etc.,
which are useful for building UI frameworks, game engines or other tools.

Fejix is designed to be a complete GUI system abstraction, resusable by all kinds of cross-platform
application, yet simple enough to port, extend or even completely reimplement.

**Features:**

- **Versatility**

    - Fejix has an extensible system of modules that allows it to support as many useful system
        features as possible.
        You want a nice shiny system API related to GUI? Suggest it to us!

- **Compatibility**

    - It is possible to port existing projects to Fejix thanks to supplementary modules
        that implement legacy and not-so-cross-platform features.
        The goal is to allow other libraries like SDL or Qt to gradually migrate to Fejix.

- **Portability**

    - Written in C99
    - All core modules are designed to be minimal, generic and platform-agnostic
    - Adding backends is simple because backends are completely independent chunks of
        code (with the exception of build scripts and some common utils)
    - Adding backends is **extremely** simple because virtually every function is optional,
        which requires all programs/toolkits to handle initialisation errors correctly, but
        this only makes them more portable themselves.


> This project is in its earliest development stage.
>
> The most interesting thing to see here is this emoji: 🌸.
>
> Yay.


## License

Copyright © 2024-2025 Mark Lagodych.
Licensed under [Apache-2.0](./LICENSE.txt)
