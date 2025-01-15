cmake_minimum_required(VERSION 3.19)

project(fejix
    VERSION 0.0.1
    DESCRIPTION "Cross-platform low-level graphical client library"
    HOMEPAGE_URL "https://github.com/FejixGUI/fejix"
    LANGUAGES "C"
)

set(KNOWN_OPTIONS
    "wayland"
    "winapi"
    "x11"
    "ram"
    "opengl"
)

# Generates project-specific variables of format "BUILD_{uppercase_option_name}"
set(BUILD_OPTIONS "" CACHE STRING ";-separated list of features to enable (any of ${KNOWN_OPTIONS})")

# This variable automatically tells CMake to build all libraries as shared
set(BUILD_SHARED_LIBS ON CACHE BOOL "Build a shared (dynamic) library instead of a static library")

set(BUILD_TESTS OFF CACHE BOOL "Build tests")

foreach(OPTION IN LISTS BUILD_OPTIONS)
    if(NOT OPTION IN_LIST KNOWN_OPTIONS)
        message(WARNING "fejix: unknown option in BUILD_OPTIONS ignored: '${OPTION}' (expected any of ${KNOWN_OPTIONS})")
        continue()
    endif()

    set(BUILD_$<UPPER_CASE:${OPTION}> ON)
    add_compile_definitions("FJ_OPT_$<UPPER_CASE:${OPTION}>")
endforeach()

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_OBJC_STANDARD 99)

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OBJC_STANDARD_REQUIRED ON)

set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_OBJC_EXTENSIONS OFF)

add_compile_options(
    "$<$<AND:$<COMPILE_LANGUAGE:C,CXX,OBJC>,$<C_COMPILER_ID:GNU,Clang,AppleClang>>:-Wall;-Wextra;-Wpedantic;-Werror=vla>"
    "$<$<AND:$<COMPILE_LANGUAGE:C,CXX>,$<C_COMPILER_ID:MSVC>>:/W4;/WX>"
)

# It is the intended way to #undef inappropriate definitions before your #includes
add_compile_definitions(
    "$<$<PLATFORM_ID:Windows,MSYS,CYGWIN>:_CRT_SECURE_NO_WARNINGS;_UNICODE;UNICODE;WIN32_LEAN_AND_MEAN>"
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


target_sources(fejix PRIVATE
    "src/core/alloc.c"
    "src/core/map.c"
    "src/core/utils.c"
    "src/core/vec.c"
    "src/common/app.c"
    "src/common/ram.c"
)


if(BUILD_WINAPI)
    target_sources(fejix PRIVATE
        "src/winapi/utils.c"
        "src/winapi/app/app.c"
        "src/winapi/image_scene/image_scene.c"
    )
endif()


if(BUILD_WAYLAND)
    set(BUILD_UTIL_UNIXSHM ON)
endif()


if(BUILD_UTIL_UNIXSHM)
    target_sources(fejix PRIVATE
        "src/util/unixshm/unixshm.c"
    )
endif()


if(BUILD_TESTS)
    enable_testing()

    add_executable(test_core_vec "tests/test_core_vec.c")
    target_link_libraries(test_core_vec fejix)
    add_test(NAME test_core_vec COMMAND core_vec_test)

    add_executable(test_core_map "tests/test_core_map.c")
    target_link_libraries(test_core_map fejix)
    add_test(NAME test_core_map COMMAND core_map_test)

    if(BUILD_UNIXSHM)
        add_executable(test_unixshm "tests/test_util_unixshm.c")
        target_link_libraries(test_unixshm fejix)
        add_test(NAME test_unixshm COMMAND unixshm_test)
    endif()

    if(BUILD_WINAPI)
        add_executable(test_winapi_utils "tests/test_winapi_utils.c")
        target_link_libraries(test_winapi_utils fejix)
        add_test(NAME test_winapi_utils COMMAND winapi_utils_test)
    endif()
endif()
