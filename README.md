# Fejix window management library

Fejix is a cross-platform window management library.
Written in C99.

> This library is in early development.
> For now, there is just an emoji: 🌸

## Build

### Fejix

```sh
mkdir build
cd build
cmake .. -D OPTION1=VALUE1 -D OPTION2=VALUE2 ...
cmake --build .
```

### Tests

```sh
cd build
cmake .. -D FEJIX_BUILD_TESTS=ON
cmake --build .
ctest 
```

### Examples

```sh
cd examples/EXAMPLE_NAME
mkdir build
cd build
cmake ..
cmake --build .
./example
```
