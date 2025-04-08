# Fejix

Fejix is a universal cross-platform GUI base library.
It wraps system APIs for window output, keyboard/mouse/other input processing etc.
and can be used to build UI frameworks, game engines and other tools.

Fejix is *not* a graphics rendering engine or a full-blown GUI framework with widgets.

Goals:

* **feature-richness**:

    No extension libraries.
    Functionality is either in Fejix or does not exist.

* **API simplicity**

    No obscure object-oriented metaprogramming magic like in GTK or Qt.
    Just simple enough C.

* **modularity**:

    Many small API modules, each of them may or may not be available at runtime.

    Makes it simple to design, extend, port, and reimplement the library.

Nice to have:

* ability to **port existing projects** to Fejix:

    Fejix has a few modules that implement legacy or not-so-cross-platform behaviors specifically
    to allow other libraries and programs to gradually migrate to it.

* **clean code**:

    ...which kinda makes sense, doesn't it? If you find something too complicated to read and
    extend, feel free to discuss it.


> Note, however, that hardly anything has been done so far.
> This library is in its earliest development stage.
> The most interesting thing to see here is this emoji: 🌸.
> Yay.


## License

Copyright 2024-2025 Mark Lagodych.
Licensed under [Apache-2.0](./LICENSE.txt>)
