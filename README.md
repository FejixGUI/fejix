# Fejix - a cross-platform low-level GUI base library

Fejix wraps low-level APIs (Windows API, Wayland, X11 etc.) to provide a common
base for cross-platform GUI toolkits, game engines and other tools.
Written in C99.

Fejix can perform tasks like window management, graphical API initialisation, input and more.

> Note, however, that hardly anything has been implemented.
>
> This library is in its earliest development stage.
>
> The most interesting thing to see here is this emoji: 🌸.
>
> Yay.


## Build

Run:

```sh
# Configure
cmake -S . -B build/ \
    -D FEJIX_FEATURES=feature1;feature2;.. \
    -D BUILD_SHARED_LIBS=ON # or OFF

# Compile
cmkake --build build/
```

## Documentation

See [documentation README](./docs/README.md).


## License

Copyright 2024-2025 Mark Lagodych.
Licensed under [Apache-2.0](./LICENSE.txt).
