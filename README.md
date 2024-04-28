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

### How to build a CMake project?

```sh
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

### Clangd support

Build Fejix library with the option: `CMAKE_EXPORT_COMPILE_COMMANDS=ON`.

#### Tests

Build Fejix library with the options: `BUILD_TESTS=ON`, `CMAKE_BUILD_TYPE=Debug`.

Run:

```sh
ctest --test-dir ./build --verbose
```

#### Examples

```sh
cd examples/{EXAMPLE_DIRECTORY}

# Build the example project
# <configure CMake>
# <build CMake>

# Run the example
./build/example
```
