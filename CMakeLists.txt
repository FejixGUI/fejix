cmake_minimum_required(VERSION 3.16)

project(
    fejix
    VERSION 0.1.0
    DESCRIPTION "Cross-platform GUI base library"
    HOMEPAGE_URL "https://github.com/FejixGUI/fejix")


include(GNUInstallDirs)


set(BUILD_SHARED_LIBS OFF CACHE BOOL "Common CMake option for default library type.")

set(fejix_build_tests OFF CACHE BOOL "Build tests for all enabled modules")

set(fejix_build_winapi OFF CACHE BOOL "Build the Windows API backend")

set(fejix_build_wayland OFF CACHE BOOL "Build the Wayland backend")

set(fejix_build_x11 OFF CACHE BOOL "Build the X11 backend")

set(fejix_detect_backends OFF CACHE BOOL "Detect the backends which are common for the target OS")

set(fejix_winapi_target_version "WIN10_RS1" CACHE STRING "Windows API NTDDI version for <sdkddkver.h>. One of: WINXP, WINVISTA, WIN7, WIN8, WINBLUE, WIN10_TH2, WIN10_RS1")

# The following variables enable some shared utils and can be enabled in backend build scripts

set(fejix_build_unix_utils OFF)


if("${fejix_detect_backends}" STREQUAL "auto")
    if("${CMAKE_SYSTEM_NAME}" MATCHES "Windows|MSYS|CYGWIN")
        set(fejix_build_winapi ON)
    elseif("${CMAKE_SYSTEM_NAME}" MATCHES "Linux|GNU|FreeBSD|OpenBSD|NetBSD|DragonFly")
        set(fejix_build_wayland ON)
        set(fejix_build_x11 ON)
    else()
        message(WARNING "Fejix could not detect backends for the target OS")
    endif()
endif()

enable_language("C")

if("${fejix_build_tests}")
    enable_testing()
endif()


if("${fejix_build_tests}")
    add_library(fejix_private_headers INTERFACE)

    target_include_directories(
        fejix_private_headers
        INTERFACE
            "${CMAKE_CURRENT_LIST_DIR}")
endif()


add_library(fejix)

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
        "$<$<C_COMPILER_ID:GNU,Clang,AppleClang>:-fvisibility=hidden;-Werror=vla;-Wall;-Wextra;-Wpedantic;-Wno-unused-function>")

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

include("${CMAKE_CURRENT_LIST_DIR}/src/shared/core/build.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/src/shared/utils/build.cmake")

include("${CMAKE_CURRENT_LIST_DIR}/src/default/build.cmake")

if("${fejix_build_winapi}")
    include("${CMAKE_CURRENT_LIST_DIR}/src/winapi/build.cmake")
endif()

if("${fejix_build_x11}")
    include("${CMAKE_CURRENT_LIST_DIR}/src/x11/build.cmake")
endif()

if("${fejix_build_unix_utils}")
    include("${CMAKE_CURRENT_LIST_DIR}/src/shared/unix/build.cmake")
endif()

configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/Doxyfile.in"
    "${CMAKE_CURRENT_LIST_DIR}/Doxyfile")

configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/include/fejix/version.h.in"
    "${CMAKE_CURRENT_LIST_DIR}/include/fejix/version.h")

install(
    DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/include/fejix"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    FILES_MATCHING PATTERN "*.h"
    PATTERN ".*" EXCLUDE)

install(
    TARGETS fejix
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")
