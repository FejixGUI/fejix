# Fejix window management library

Fejix is a cross-platform C99 library that abstracts graphical protocols.
It can perform tasks like window management, graphical API initialisation,
input and more.

> This library is in the earliest development stage.
>
> The most interesting thing to see here is this emoji: 🌸.
>
> Yay.

## Build

### How to build a CMake project?

```sh
# Make the project directory current
cd {PROJECT_DIRECTORY}

# Folder where to build the project
mkdir ./build

# Configure the build
cmake -S . -B ./build
# Or if you need to specify some options:
cmake -S . -B ./build -D {OPTION1}={VALUE1} -D {OPTION2}={VALUE2}

# Perform the build
cmake --build ./build
```

### How to build Fejix components?

#### Fejix library

Build Fejix library as a CMake project.

#### Tests

Build Fejix library with the options: `BUILD_TESTS=ON`, `CMAKE_BUILD_TYPE=Debug`.

Run:

```sh
ctest --test-dir ./build --verbose
```

#### Examples

1. Change directory to `examples/{EXAMPLE_DIRECTORY}`.
2. Build the chosen example project.
3. Run `./build/example`.
