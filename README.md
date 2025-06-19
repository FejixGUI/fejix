# Fejix

**Fejix** is a cross-platform **GUI base** library.

It wraps system APIs (Windows API, Wayland, X11, Cocoa, etc.)
to provide access to low-level GUI features like
window management, input handling, system notifications, tray icons and much more.

This library is useful for building UI frameworks, game engines or other tools.

**Features:**

- **Batteries included**

    - Fejix has an extensible system of modules that allows it to support as many useful system
        features as possible.
        You want a nice shiny system API related to GUI? Suggest it to us!

- **Compatible**

    - It is possible to port existing projects to Fejix thanks to supplementary modules
        that implement legacy or not-so-cross-platform features.
        This would allow other libraries like SDL or Qt to gradually migrate to Fejix.

- **Portable**

    - Written in C99
    - All core modules are designed to be cross-platform and generic
    - Adding backends is extremely simple because backends are completely independent chunks of
        code (with the exception of build scripts and the backend loading function)


> This project is in its earliest development stage.
>
> The most interesting thing to see here is this emoji: 🌸.
>
> Yay.


## License

Copyright © 2024-2025 Mark Lagodych.
Licensed under [Apache-2.0](./LICENSE.txt)
