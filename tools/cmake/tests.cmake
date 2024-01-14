option(BUILD_TESTS OFF)

if(BUILD_TESTS)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(
        FATAL_ERROR
        "***** You must specify CMAKE_BUILD_TYPE=\"Debug\" to enable tests"
    )
endif()

enable_testing()

include_directories("${FEJIX_INCLUDE}")
include_directories("${FEJIX_ROOT}")

macro(fejix_add_simple_c_test test_name c_source_file)
    add_executable(${test_name} "${c_source_file}")
    target_link_libraries(${test_name} fejix)
    add_test(NAME ${test_name} COMMAND "${test_name}")
endmacro()

fejix_add_simple_c_test(core_vec_test "${FEJIX_TESTS}/core/vec.c")

if(FJ_OPT_WINAPI)
    fejix_add_simple_c_test(winapi_test "${FEJIX_TESTS}/winapi/utils.c")
endif()

endif()
