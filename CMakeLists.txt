cmake_minimum_required(VERSION 3.19)

project(fejix
    VERSION 0.0.1
    DESCRIPTION "Graphical client library"
    HOMEPAGE_URL "https://github.com/FejixGUI/fejix"
    LANGUAGES "C"
)

set(FEJIX_KNOWN_FEATURES
    "wayland" "winapi" "x11"
    "softer" "opengl"
)

set(FEJIX_FEATURES "" CACHE STRING
    ";-separated list of features to enable (any of ${FEJIX_KNOWN_FEATURES})")

set(FEJIX_BUILD_TESTS OFF CACHE BOOL "Build tests")

foreach(FEATURE IN LISTS FEJIX_FEATURES)
    if(NOT FEATURE IN_LIST FEJIX_KNOWN_FEATURES)
        message(WARNING
            "[fejix] unknown library feature in FEJIX_FEATURES ignored: \"${FEATURE}\""
            " (expected any of ${FEJIX_KNOWN_FEATURES})"
        )
        continue()
    endif()

    add_compile_definitions("FJ_OPT_$<UPPER_CASE:${FEATURE}>")
endforeach()

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_OBJC_STANDARD 99)

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OBJC_STANDARD_REQUIRED ON)

set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_OBJC_EXTENSIONS OFF)

add_compile_options(
    "$<$<AND:$<COMPILE_LANGUAGE:C,CXX,OBJC>,$<C_COMPILER_ID:GNU,Clang,AppleClang>>:-Wall;-Wextra;-Wpedantic;-Wunused;>"
    "$<$<AND:$<COMPILE_LANGUAGE:C>,$<C_COMPILER_ID:GNU,Clang,AppleClang>>:-Werror=vla>"
    "$<$<AND:$<COMPILE_LANGUAGE:C,CXX>,$<C_COMPILER_ID:MSVC>>:/W4;/WX>"
)

add_compile_definitions(
    "$<$<PLATFORM_ID:Windows,MSYS,CYGWIN>:_CRT_SECURE_NO_WARNINGS;_UNICODE;UNICODE>"
    "$<$<PLATFORM_ID:Linux>:_GNU_SOURCE>"
)

include_directories(".")


# Main target
add_library(fejix)

target_include_directories(fejix PUBLIC "include")

target_compile_definitions(fejix PRIVATE "FJ_OPT_INTERNAL")

# These flags specify what modules to build
set(FEJIX_BUILD_WAYLAND OFF)
set(FEJIX_BUILD_WAYLAND_SOFTER OFF)
set(FEJIX_BUILD_WINAPI OFF)
set(FEJIX_BUILD_UNIXSHM OFF)

if("wayland" IN_LIST FEJIX_FEATURES)
    set(FEJIX_BUILD_WAYLAND ON)

    if("softer" IN_LIST FEJIX_FEATURES)
        set(FEJIX_BUILD_WAYLAND_SOFTER ON)
        set(FEJIX_BUILD_UNIXSHM ON)
    endif()
endif()

if("winapi" IN_LIST FEJIX_FEATURES)
    set(FEJIX_BUILD_WINAPI ON)
endif()


target_sources(fejix PRIVATE
    "src/implementation.c"

    "src/core/alloc.c"
    "src/core/base.c"
    "src/core/map.c"
    "src/core/utils.c"
    "src/core/vec.c"
)

if(FEJIX_BUILD_WINAPI)
    target_sources(fejix PRIVATE
        "src/winapi/implementation.c"
        "src/winapi/utils.c"
        "src/winapi/client/client.c"
        "src/winapi/sleep_timer/sleep_timer.c"
        "src/winapi/window/window.c"
    )
endif()

if(FEJIX_BUILD_UNIXSHM)
    target_sources(fejix PRIVATE
        "src/unixshm/unixshm.c"
    )
endif()


if(FEJIX_BUILD_TESTS)
    enable_testing()

    add_executable(core_vec_test "tests/core/vec.c")
    target_link_libraries(core_vec_test fejix)
    add_test(NAME core_vec_test COMMAND core_vec_test)

    add_executable(core_map_test "tests/core/map.c")
    target_link_libraries(core_map_test fejix)
    add_test(NAME core_map_test COMMAND core_map_test)

    if(FEJIX_BUILD_UNIXSHM)
        add_executable(unixshm_test "tests/unixshm/test.c")
        target_link_libraries(unixshm_test fejix)
        add_test(NAME unixshm_test COMMAND unixshm_test)
    endif()

    if(FEJIX_BUILD_WINAPI)
        add_executable(winapi_utils_test "tests/winapi/utils.c")
        target_link_libraries(winapi_utils_test fejix)
        add_test(NAME winapi_utils_test COMMAND winapi_utils_test)
    endif()
endif()
