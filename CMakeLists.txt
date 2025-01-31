cmake_minimum_required(VERSION 3.19)

project(fejix
    VERSION 0.0.1
    DESCRIPTION "Cross-platform low-level graphical client library"
    HOMEPAGE_URL "https://github.com/FejixGUI/fejix"
    LANGUAGES "C")

set(KNOWN_OPTIONS
    "wayland"
    "winapi"
    "x11"
    "ram"
    "opengl")

# Generates project-specific variables of format "BUILD_{uppercase_option_name}"
set(BUILD_OPTIONS "" CACHE STRING
    ";-separated list of features to enable (any of ${KNOWN_OPTIONS})")

# This variable automatically tells CMake to build all libraries as shared
set(BUILD_SHARED_LIBS ON CACHE BOOL
    "Build a shared (dynamic) library instead of a static library")

set(BUILD_TESTS OFF CACHE BOOL
    "Build tests")

set(WINDOWS_BUILD_VERSION "" CACHE STRING
    "Target Microsoft Windows build version (e.g. 18362 for WIN10_19H1, autodetected if empty)")


# Check if library options are valid
# Defines the corresponding CMake variables and compile definitions
foreach(OPTION IN LISTS BUILD_OPTIONS)
    if(NOT OPTION IN_LIST KNOWN_OPTIONS)
        message(WARNING
            "fejix: unknown option in BUILD_OPTIONS ignored: '${OPTION}'"
            " (expected any of ${KNOWN_OPTIONS})")
        continue()
    endif()

    set(BUILD_$<UPPER_CASE:${OPTION}> ON)
    add_compile_definitions("FJ_OPT_$<UPPER_CASE:${OPTION}>")
endforeach()


# Selects the target Windows build version.
# Defines WINDOWS_VERSION_DETAILED_ID and WINDOWS_VERSION_ID
if ("${CMAKE_SYSTEM_NAME}" MATCHES "Windows|MSYS|CYGWIN")
    set(MINIMAL_WINDOWS_VERSION 2600)

    # Autodetect target Windows build version, if necessary
    if("${WINDOWS_BUILD_VERSION}" STREQUAL "")
        if("${CMAKE_SYSTEM_NAME}" MATCHES "Windows")
            set(WINDOWS_ENVIRONMENT_VERSION "${CMAKE_SYSTEM_VERSION}")
        elseif("${CMAKE_SYSTEM_NAME}" MATCHES "MSYS|CYGWIN")
            execute_process(
                COMMAND "uname" "--kernel-name"
                OUTPUT_VARIABLE WINDOWS_ENVIRONMENT_VERSION
                OUTPUT_STRIP_TRAILING_WHITESPACE
                ERROR_QUIET
            )
        endif()

        if("${WINDOWS_ENVIRONMENT_VERSION}" STRGREATER "")
            message(STATUS
                "fejix: detected Windows environment version ${WINDOWS_ENVIRONMENT_VERSION}")

            string(REGEX MATCH "[0-9]+$" WINDOWS_BUILD_VERSION "${WINDOWS_ENVIRONMENT_VERSION}")

            message(STATUS
                "fejix: detected Windows build version ${WINDOWS_BUILD_VERSION}")
        else()
            message(WARNING
                "fejix: cannot detect Windows build version,"
                " falling back to ${MINIMAL_WINDOWS_VERSION}!")

            set(WINDOWS_BUILD_VERSION "${MINIMAL_WINDOWS_VERSION}")
        endif()
    endif()

    if ("${WINDOWS_BUILD_VERSION}" LESS "${MINIMAL_WINDOWS_VERSION}")
        message(WARNING
            "fejix: selecting a minimal Windows build version ${MINIMAL_WINDOWS_VERSION}")

        set(WINDOWS_BUILD_VERSION "${MINIMAL_WINDOWS_VERSION}")
    endif()

    set(WINDOWS_BUILD_VERSIONS
        18362 17763 17134 16299 15063 14393 10586 10240
        9600 9200 7601 6002 2600)
    set(WINDOWS_VERSION_DETAILED_IDS
        WIN10_19H1 WIN10_RS5 WIN10_RS4 WIN10_RS3 WIN10_RS2 WIN10_RS1 WIN10_TH2 WIN10
        WINBLUE WIN8 WIN7 VISTA WINXP)

    foreach(BUILD_VERSION VERSION_DETAILED_ID IN ZIP_LISTS
            WINDOWS_BUILD_VERSIONS WINDOWS_VERSION_DETAILED_IDS)

        if("${WINDOWS_BUILD_VERSION}" GREATER_EQUAL "${BUILD_VERSION}")
            message(STATUS "fejix: targeting Windows ${VERSION_DETAILED_ID} and newer")

            string(REGEX MATCH "^[a-zA-Z0-9]+" VERSION_ID "${VERSION_DETAILED_ID}")

            set(WINDOWS_VERSION_DETAILED_ID "${VERSION_DETAILED_ID}")
            set(WINDOWS_VERSION_ID "${VERSION_ID}")

            break()
        endif()

    endforeach()
endif()


function(generate_implementation_for_interface INTERFACE_FILE OUTPUT_FILE)
    file(READ "${INTERFACE_FILE}" FILE_TEXT)
    set(OUTPUT_TEXT "")

    set(FUNCTION_REGEX "FJ_PUBLIC[ \n]*([^\\(]+)\\(([^\\)]*)\\)")
    string(REGEX MATCHALL "${FUNCTION_REGEX}" FUNCTIONS "${FILE_TEXT}")
    foreach(FUCNTION IN LISTS FUNCTIONS)
        string(REGEX MATCH "${FUNCTION_REGEX}" IGNORED_OUTPUT "${FUCNTION}")
        set(BEFORE_PAREN "${CMAKE_MATCH_1}")
        set(ARGUMENT_DEFINITION_LIST "${CMAKE_MATCH_2}")

        string(REGEX MATCH "(.+[^_a-zA-Z0-9])([_a-zA-Z0-9]+)$" IGNORED_OUTPUT "${BEFORE_PAREN}")
        set(RETURN_TYPE "${CMAKE_MATCH_1}")
        set(FUNCTION_NAME "${CMAKE_MATCH_2}")
        string(STRIP "${RETURN_TYPE}" RETURN_TYPE)

        set(ARGUMENT_DEFINITIONS "${ARGUMENT_DEFINITION_LIST}")
        string(REPLACE "," ";" ARGUMENT_DEFINITIONS "${ARGUMENT_DEFINITIONS}")
        set(ARGUMENT_PASS_LIST "")
        foreach(ARGUMENT_DEFINITION IN LISTS ARGUMENT_DEFINITIONS)
            string(REGEX MATCH "([_a-zA-Z0-9]+)$" IGNORED_OUTPUT "${ARGUMENT_NAME}")
            list(APPEND ARGUMENT_PASS_LIST "${CMAKE_MATCH_1}")
        endforeach()

        string(CONCAT OUTPUT_TEXT
            "${OUTPUT_TEXT}"
            "\n"
            "")
    endforeach()
endfunction()

generate_implementation_for_interface("include/fejix/app.h")


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
    "$<$<AND:$<COMPILE_LANGUAGE:C,CXX>,$<C_COMPILER_ID:MSVC>>:/W4;/WX>")

set(WINDOWS_DEFINITIONS
    "_CRT_SECURE_NO_WARNINGS"
    "_UNICODE"
    "UNICODE"
    "WIN32_LEAN_AND_MEAN"
    "NTDDI_VERSION=NTDDI_VERSION_${WINDOWS_VERSION_DETAILED_ID}"
    "_WIN32_WINNT=_WIN32_WINNT_${WINDOWS_VERSION_ID}")

add_compile_definitions(
    "$<$<PLATFORM_ID:Windows,MSYS,CYGWIN>:${WINDOWS_DEFINITIONS}>"
    "$<$<PLATFORM_ID:Linux>:_GNU_SOURCE>")

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
