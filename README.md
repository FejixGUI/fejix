# Fejix

Fejix is a low-level cross-platform GUI base library.
It wraps system APIs for window output, keyboard, mouse and other input processing etc.
and can be used to build UI frameworks, game engines and other tools.

Fejix is *not* a graphics rendering engine or a widget framework.

Features:

* **Feature-rich**:

    No extension libraries. Fejix has all the functionality you need.

* **Simple API**:

    It's just C. Use of metaprogramming in public code is limited to just three simple macros or so.

* **Modular API**:

    The library is divided into many small modules, and each of them may or may not be available at
    runtime depending on the platform features and the current support of those features in Fejix.

    This makes it simpler to design, extend, port, and reimplement the library.

* **Modular backends**:

    All backends directly expose their functions to the user, no wrappers or abstractions.
    Thus backends are completely independent of each other and are fairly easy to add,
    replace, enable/disable for compilation, and select at runtime.

Secondary features:

* **Portability of existing projects**:

    You can straightforwardly port other projects to Fejix.
    The library has a few modules that implement legacy or not-so-cross-platform behaviors
    specifically to allow other libraries and programs depending on those behaviors to gradually
    migrate to Fejix.

* **Clean code**:

    ...to the extent that it remains fairly easy to add nice shiny features to the library, because
    there a *lot* of nice shiny features in the giant fragmented world of graphical operating
    systems.

> This library is in its earliest development stage.
> The most interesting thing to see here is this emoji: 🌸.
> Yay.


## License

Copyright 2024-2025 Mark Lagodych.
Licensed under [Apache-2.0](./LICENSE.txt>)
