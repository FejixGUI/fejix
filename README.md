# Fejix window management library

Fejix is a cross-platform window management library.
Written in C99.

> This library is in early development.
> For now, there is just an emoji: 🌸

## Build

### Fejix

```sh
mkdir build
cmake -B build -D OPTION1=VALUE1 -D OPTION2=VALUE2 ...
cmake --build build
```

### Tests

```sh
cmake -B build -D FEJIX_BUILD_TESTS=ON
cmake --build build
ctest 
```

### Examples

```sh
cd examples/EXAMPLE_NAME
mkdir build
cmake -B build
cmake --build build
./example
```
