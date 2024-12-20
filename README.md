# Fejix graphical client library

Fejix is a cross-platform C99 base library for clients of graphical shells.
Its goal is to wrap graphical shell protocols (Windows API, Wayland, X11 etc.) to provide a common
base for GUI toolkits.

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

### Library

```sh
cmake -S . -B build -D FEJIX_FEATURES="feature1;feature2;feature3"
cmkake --build build
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
