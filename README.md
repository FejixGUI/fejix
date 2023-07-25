# Fejix window management library

Fejix is a low-level window management library in early development.

For now, there is just an emoji: 🌸

## Build

```sh
# Configure
cmake -B build -D OPTION1=VALUE1 -D OPTION2=VALUE2 ...
# Build
cmake --build build
```

## Test

First, build the project with `FEJIX_BUILD_TESTS=ON`.

```sh
ctest --test-dir build
```

Altogether:

```sh
cmake --build build && ctest --test-dir build 
```