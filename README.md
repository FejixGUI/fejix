# Fejix window management library

Fejix is a cross-platform window management library.
Written in C99.

> This library is in early development.
> For now, there is just an emoji: 🌸

## Build

## How to build a CMake project?

```sh
# Make the project directory current
cd {PROJECT_DIRECTORY}

# Folder where to build the project
mkdir build

# Configure the build
cmake -S . -B build
# Or if you need to specify any options:
cmake -S . -B build -D {OPTION1}={VALUE1} -D {OPTION2}={VALUE2}

# Perform the build
cmake --build build
```

## How to build Fejix components?

### Fejix library

Build Fejix library as a CMake project:

```sh
mkdir build
cmake -S . -B build {options...}
cmake --build build
```

### Tests

Build Fejix library with the option: `FEJIX_BUILD_TESTS=ON`.

### Examples

1. Change directory to `examples/{EXAMPLE_DIRECTORY}`.
2. Build the chosen example project.
3. Run `./build/example`.
