cmake_minimum_required(VERSION 3.16)

project(
    fejix
    VERSION 0.0.1
    DESCRIPTION "Cross-platform low-level graphical client library"
    HOMEPAGE_URL "https://github.com/FejixGUI/fejix")


include(GNUInstallDirs)


set(BUILD_SHARED_LIBS OFF CACHE BOOL "Common CMake option for default library type. If loader is enabled, it makes other implementations build as shared.")

set(fejix_detect_implementations ON CACHE BOOL "Enable the implementations common for the target OS")

set(fejix_build_tests OFF CACHE BOOL "Build tests for all enabled modules")

set(fejix_build_loader OFF CACHE BOOL "Build loader implementation")

set(fejix_loader_default_libraries "" CACHE STRING ";-separated list of 'winapi', 'x11', 'wayland'... (TODO)")

set(fejix_loader_detect_default_libraries ON CACHE BOOL "Build the default library paths into the loader according to the enabled implementations.")

set(fejix_build_winapi OFF CACHE BOOL "Build Windows API implementation")

set(fejix_winapi_ntddi_version "WIN10_RS1" CACHE STRING "Windows API NTDDI version for <sdkddkver.h>. One of: WINXP, WINVISTA, WIN7, WIN8, WINBLUE, WIN10_TH2, WIN10_RS1")



if("${fejix_detect_implementations}")
    if("${CMAKE_SYSTEM_NAME}" MATCHES "^Windows$|MSYS|Cygwin")
        set(fejix_build_winapi ON)
    elseif("${CMAKE_SYSTEM_NAME}" MATCHES "Linux|GNU|BSD|DragonFly")
        set(fejix_build_loader ON)
        set(fejix_build_wayland ON)
        set(fejix_build_x11 ON)
    endif()
endif()

if("${fejix_build_loader}" AND "${fejix_loader_detect_default_libraries}")
    if("${fejix_build_winapi}")
        list(APPEND fejix_loader_default_libraries "winapi")
    endif()
    if("${fejix_build_x11}")
        list(APPEND fejix_loader_default_libraries "x11")
    endif()
    if("${fejix_build_wayland}")
        list(APPEND fejix_loader_default_libraries "wayland")
    endif()
endif()

if("${BUILD_SHARED_LIBS}")
    set(fejix_implementation_target_type "SHARED")
else()
    set(fejix_implementation_target_type "STATIC")
endif()

if("${fejix_build_loader}")
    set(fejix_loader_target_type "${fejix_implementation_target_type}")
    set(fejix_implementation_target_type "SHARED")
endif()

set(fejix_root_path "${CMAKE_CURRENT_SOURCE_DIR}")

enable_language("C")

if("${fejix_build_tests}")
    enable_testing()
endif()

include("${fejix_root_path}/src/headers/build.cmake")
include("${fejix_root_path}/src/core/build.cmake")
include("${fejix_root_path}/src/default/build.cmake")

if("${fejix_build_loader}")
    include("${fejix_root_path}/src/loader/build.cmake")
endif()

if("${fejix_build_winapi}")
    include("${fejix_root_path}/src/winapi/build.cmake")
endif()
