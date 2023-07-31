# Fejix window management library

Fejix is a low-level window management library in early development.

For now, there is just an emoji: 🌸

## Build

```sh
mkdir build
cd build
cmake .. -D OPTION1=VALUE1 -D OPTION2=VALUE2 ...
ninja # Or 'make' or whatever cmake uses
```

## Test

```sh
cd build
cmake .. -D FEJIX_BUILD_TESTS=ON
ninja
ctest 
```

## Build and run examples

```sh
cd examples/EXAMPLE_NAME
mkdir build
cd build
cmake ..
ninja
./example
```