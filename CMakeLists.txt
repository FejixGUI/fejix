cmake_minimum_required(VERSION 3.16)
project(fejix VERSION 0.0.1)

include(GNUInstallDirs)

# ==============================================================================
# BUILD OPTIONS
# ==============================================================================

set(BUILD_SHARED_LIBS OFF CACHE BOOL
    "Standard CMake option that sets the library type.")

set(fejix_build_tests OFF CACHE BOOL
    "Build tests for all enabled modules")

set(fejix_enable_errors ON CACHE BOOL
    "Enables the library to set the last error message")

set(fejix_enable_error_locations ON CACHE BOOL
    "Adds the file name, line and function to every error message")

set(fejix_detect_platforms OFF CACHE BOOL
    "Detect the platforms which are common for the target OS")

set(fejix_build_winapi OFF CACHE BOOL
    "Build the Windows API platform")

set(fejix_winapi_target_version "WIN10_RS1" CACHE STRING
    "Windows API NTDDI version for <sdkddkver.h>. One of: WINXP, WINVISTA, WIN7, WIN8, WINBLUE, WIN10_TH2, WIN10_RS1")

set(fejix_build_wayland OFF CACHE BOOL
    "Build the Wayland platform")

set(fejix_build_x11 OFF CACHE BOOL
    "Build the X11 platform")

# The following variables enable some shared utils and can be enabled in
# platform build scripts
set(fejix_build_unix_utils OFF)

# ==============================================================================
# PLATFORM DETECTION
# ==============================================================================

if("${fejix_detect_platforms}" STREQUAL "auto")
    if("${CMAKE_SYSTEM_NAME}" MATCHES "Windows|MSYS|CYGWIN")
        set(fejix_build_winapi ON)
    elseif("${CMAKE_SYSTEM_NAME}" MATCHES "Linux|GNU|FreeBSD|OpenBSD|NetBSD|DragonFly")
        set(fejix_build_wayland ON)
        set(fejix_build_x11 ON)
    else()
        message(WARNING "Fejix could not detect platforms for the target OS")
    endif()
endif()

# ==============================================================================
# BUILD SETUP
# ==============================================================================

enable_language("C")

add_library(fejix)

install(
    DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/include/fejix"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    FILES_MATCHING PATTERN "*.h"
    PATTERN ".*" EXCLUDE)

install(
    TARGETS fejix
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")

set_target_properties(
    fejix
    PROPERTIES
    VERSION
        "${CMAKE_PROJECT_VERSION}"
    SOVERSION
        "${CMAKE_PROJECT_VERSION_MAJOR}")

target_compile_features(
    fejix
    PRIVATE
        "c_std_99")

target_compile_options(
    fejix
    PRIVATE
        "$<$<C_COMPILER_ID:MSVC>:/W4>"
        "$<$<C_COMPILER_ID:GNU,Clang,AppleClang>:-fvisibility=hidden;-Werror=vla;-Wall;-Wextra;-Wpedantic>")

target_compile_definitions(
    fejix
    PRIVATE
        "FJ_COMPILE_OPT_PRIVATE_CODE"
        "$<$<C_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>")

if("${BUILD_SHARED_LIBS}" AND "${CMAKE_SYSTEM_NAME}" MATCHES "Windows|WindowsStore|MSYS|CYGWIN|WindowsCE|WindowsPhone")
    target_compile_definitions(
        fejix
        PRIVATE
            "FJ_COMPILE_OPT_DLLEXPORT"
        INTERFACE
            "FJ_INCLUDE_OPT_DLLIMPORT")
endif()

target_include_directories(
    fejix
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}/include"
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}")

target_sources(
    fejix
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/src/platform.c")


configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/docs/version.py.in"
    "${CMAKE_CURRENT_LIST_DIR}/docs/version.py")

configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/include/fejix/version.h.in"
    "${CMAKE_CURRENT_LIST_DIR}/include/fejix/version.h")


if("${fejix_build_tests}")
    enable_testing()

    add_library(fejix_private_headers INTERFACE)

    target_include_directories(
        fejix_private_headers
        INTERFACE
            "${CMAKE_CURRENT_LIST_DIR}")
endif()

# ==============================================================================
# PLATFORMS
# ==============================================================================

if("${fejix_build_winapi}")
    include("${CMAKE_CURRENT_LIST_DIR}/src/winapi/build.cmake")
endif()

if("${fejix_build_x11}")
    include("${CMAKE_CURRENT_LIST_DIR}/src/x11/build.cmake")
endif()

# ==============================================================================
# UTILS
# ==============================================================================

# These utils are always built
include("${CMAKE_CURRENT_LIST_DIR}/src/shared/common/build.cmake")


# Utils can themselves enable other utils on which they depend.
# Here the order of inclusion is important.

if("${fejix_build_unix_utils}")
    include("${CMAKE_CURRENT_LIST_DIR}/src/shared/unix/build.cmake")
endif()

