cmake_minimum_required(VERSION 3.19)

project(fejix
    VERSION 0.0.1
    DESCRIPTION "Graphical client library"
    HOMEPAGE_URL "https://github.com/FejixGUI/fejix"
    LANGUAGES "C"
)

set(FEJIX_KNOWN_FEATURES
    "winapi" "wayland" "x11"
    "softer" "opengl"
)

set(FEJIX_FEATURES "" CACHE STRING "Semi-colon-separated list of features to enable (possible values are ${FEJIX_KNOWN_FEATURES})")
set(FEJIX_BUILD_TESTS OFF CACHE BOOL "Build tests")

foreach(FEATURE IN LISTS FEJIX_FEATURES)
    if(NOT FEATURE IN_LIST FEJIX_KNOWN_FEATURES)
        message(WARNING "[fejix] unknown library feature in FEJIX_FEATURES: \"${FEATURE}\" (expected ${FEJIX_KNOWN_FEATURES})")
        continue()
    endif()

    string(TOUPPER FEATURE FEATURE)
    add_compile_definitions("FJ_OPT_${FEATURE}")
endforeach()

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(CMAKE_C_COMPILER_ID MATCHES "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options("-Wall" "-Wextra" "-Wpedantic" "-Wunused" "-Werror=vla")
elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    add_compile_options("/W4" "/WX")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows" OR CMAKE_SYSTEM_NAME STREQUAL "MSYS")
    add_compile_definitions("_CRT_SECURE_NO_WARNINGS" "_UNICODE" "UNICODE")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_compile_definitions("_GNU_SOURCE")
endif()

include_directories(".")


# Main target
add_library(fejix)

target_include_directories(fejix PUBLIC "include")

target_compile_definitions(fejix PRIVATE "FJ_OPT_INTERNAL")

target_sources(fejix PRIVATE
    "src/implementation.c"

    "src/core/alloc.c"
    "src/core/base.c"
    "src/core/map.c"
    "src/core/utils.c"
    "src/core/vec.c"
)

if("winapi" IN_LIST FEJIX_FEATURES)
    target_sources(fejix PRIVATE
        "src/winapi/implementation.c"
        "src/winapi/utils.c"
        "src/winapi/client/client.c"
        "src/winapi/sleep_timer/sleep_timer.c"
        "src/winapi/window/window.c"
    )
endif()

if(FEJIX_BUILD_TESTS)
    enable_testing()

    if("winapi" IN_LIST FEJIX_FEATURES)
        add_executable(winapi_utils_test "tests/winapi/utils.c")
        target_link_libraries(winapi_utils_test fejix)
        add_test(NAME winapi_utils_test COMMAND winapi_utils_test)
    endif()
endif()
