# Fejix window management library

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