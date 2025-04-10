cmake_minimum_required(VERSION 3.16)

project(
    fejix
    VERSION 0.1.0
    DESCRIPTION "Cross-platform low-level GUI base library"
    HOMEPAGE_URL "https://github.com/FejixGUI/fejix")


include(GNUInstallDirs)


set(BUILD_SHARED_LIBS OFF CACHE BOOL "Common CMake option for default library type.")

set(fejix_build_tests OFF CACHE BOOL "Build tests for all enabled modules")

set(fejix_backends "" CACHE STRING "The backends to enable. If \"detect\", selects the backends common for the target OS")

set(fejix_winapi_target_version "WIN10_RS1" CACHE STRING "Windows API NTDDI version for <sdkddkver.h>. One of: WINXP, WINVISTA, WIN7, WIN8, WINBLUE, WIN10_TH2, WIN10_RS1")

if("${fejix_backends}" STREQUAL "detect")
    if("${CMAKE_SYSTEM_NAME}" MATCHES "^Windows$|MSYS|Cygwin")
        set(fejix_backends "winapi")
    elseif("${CMAKE_SYSTEM_NAME}" MATCHES "Linux|GNU|BSD|DragonFly")
        set(fejix_backends "wayland" "x11")
    else()
        message(WARNING "Fejix could not detect backends for the target OS")
        set(fejix_backends "")
    endif()
endif()

list(LENGTH fejix_backends fejix_backend_count)

enable_language("C")

if("${fejix_build_tests}")
    enable_testing()
endif()


add_library(fejix)

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
        "FJ_BUILDING_PRIVATE_CODE"
        "$<$<C_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>")

if("${fejix_backend_count}" EQUAL 1)
    target_compile_definitions(
        fejix
        PRIVATE
            "FJ_BUILDING_SINGLE_BACKEND")
endif()

target_include_directories(
    fejix
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}/include"
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}")


include("${CMAKE_CURRENT_LIST_DIR}/src/build.cmake")

include("${CMAKE_CURRENT_LIST_DIR}/src/utils/build.cmake")

if("winapi" IN_LIST fejix_backends)
    include("${CMAKE_CURRENT_LIST_DIR}/winapi/build.cmake")
endif()

install(
    DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/include/fejix"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    FILES_MATCHING PATTERN "*.h"
    PATTERN ".*" EXCLUDE)

install(
    TARGETS fejix
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")
