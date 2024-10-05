# Fejix window management library

Fejix is a cross-platform C99 library that abstracts graphical compositor
protocols (Windows API, Wayland or X11 are examples of such protocols).
Fejix can perform tasks like window management, graphical API initialisation,
input and more.

> Note, however, that hardly anything has been implemented.
>
> This library is in its earliest development stage.
>
> The most interesting thing to see here is this emoji: 🌸.
>
> Yay.

## Build

### The library

```sh
meson setup build
meson configure build -D implementations=impl1,impl2,impl3 -D features=feat1,feat2,feat3
meson compile -C build
```

### Documentation

Prerequisites:
* Python 3
* [Sphinx](https://www.sphinx-doc.org/en/master/) - documentation generator
* [Hawkmoth](https://github.com/jnikula/hawkmoth) - Sphinx C extension
* [Furo](https://github.com/pradyunsg/furo) - Sphinx theme

Run:
```sh
cd docs
make html
```
