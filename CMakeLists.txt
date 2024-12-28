cmake_minimum_required(VERSION 3.19)

project(fejix
    VERSION 0.0.1
    DESCRIPTION "Cross-platform graphical client library"
    HOMEPAGE_URL "https://github.com/FejixGUI/fejix"
    LANGUAGES "C"
)

set(KNOWN_FEATURES
    "wayland"
    "winapi"
    "x11"
    "softer"
    "opengl"
)

set(FEJIX_FEATURES "" CACHE STRING
    ";-separated list of features to enable (any of ${KNOWN_FEATURES})"
)

# This variable automatically tells CMake to build all libraries as shared
set(BUILD_SHARED_LIBS ON CACHE BOOL "Build a shared (dynamic) library instead of a static library")

set(BUILD_TESTS OFF CACHE BOOL "Build tests")

foreach(FEATURE IN LISTS FEJIX_FEATURES)
    if(NOT FEATURE IN_LIST KNOWN_FEATURES)
        message(WARNING
            "[fejix] unknown library feature in FEJIX_FEATURES ignored: \"${FEATURE}\""
            " (expected any of ${KNOWN_FEATURES})"
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
    "$<$<AND:$<COMPILE_LANGUAGE:C,CXX,OBJC>,$<C_COMPILER_ID:GNU,Clang,AppleClang>>:-Wall;-Wextra;-Wpedantic;-Wunused;-Werror=vla>"
    "$<$<AND:$<COMPILE_LANGUAGE:C,CXX>,$<C_COMPILER_ID:MSVC>>:/W4;/WX>"
)

add_compile_definitions(
    "$<$<PLATFORM_ID:Windows,MSYS,CYGWIN>:_CRT_SECURE_NO_WARNINGS;_UNICODE;UNICODE>"
    "$<$<PLATFORM_ID:Linux>:_GNU_SOURCE>"
)

include_directories(".")


# Main target
add_library(fejix)

set_target_properties(fejix PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

target_include_directories(fejix PUBLIC "include")

target_compile_definitions(fejix PRIVATE "FJ_OPT_INTERNAL")

# These flags specify what modules to build
set(BUILD_WAYLAND OFF)
set(BUILD_WAYLAND_SOFTER OFF)
set(BUILD_WINAPI OFF)
set(BUILD_UNIXSHM OFF)

if("wayland" IN_LIST FEJIX_FEATURES)
    set(BUILD_WAYLAND ON)

    if("softer" IN_LIST FEJIX_FEATURES)
        set(BUILD_WAYLAND_SOFTER ON)
        set(BUILD_UNIXSHM ON)
    endif()
endif()

if("winapi" IN_LIST FEJIX_FEATURES)
    set(BUILD_WINAPI ON)
endif()


target_sources(fejix PRIVATE
    "src/core/alloc.c"
    "src/core/map.c"
    "src/core/utils.c"
    "src/core/vec.c"
    "src/generic/client.c"
    "src/generic/softer.c"
)

if(BUILD_WINAPI)
    target_sources(fejix PRIVATE
        "src/winapi/utils.c"
        "src/winapi/client/client.c"
        "src/winapi/window/window.c"
    )
endif()

if(BUILD_UNIXSHM)
    target_sources(fejix PRIVATE
        "src/unixshm/unixshm.c"
    )
endif()


if(BUILD_TESTS)
    enable_testing()

    add_executable(test_core_vec "tests/core/vec.c")
    target_link_libraries(test_core_vec fejix)
    add_test(NAME test_core_vec COMMAND core_vec_test)

    add_executable(test_core_map "tests/core/map.c")
    target_link_libraries(test_core_map fejix)
    add_test(NAME test_core_map COMMAND core_map_test)

    if(BUILD_UNIXSHM)
        add_executable(test_unixshm "tests/unixshm/test.c")
        target_link_libraries(test_unixshm fejix)
        add_test(NAME test_unixshm COMMAND unixshm_test)
    endif()

    if(BUILD_WINAPI)
        add_executable(test_winapi_utils "tests/winapi/utils.c")
        target_link_libraries(test_winapi_utils fejix)
        add_test(NAME test_winapi_utils COMMAND winapi_utils_test)
    endif()
endif()
