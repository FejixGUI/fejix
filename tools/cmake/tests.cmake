if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(FATAL_ERROR "Building tests requires CMAKE_BUILD_TYPE=\"Debug\"")
endif()

enable_testing()

macro(fejix_add_test test_name source_file)
    add_executable(${test_name} "${source_file}")
    target_link_libraries(${test_name} fejix)
    add_test(NAME ${test_name} COMMAND ${test_name})
endmacro()

fejix_add_test(fejix_test_core_list "${FEJIX_TESTS}/core/list.c")
fejix_add_test(fejix_test_client_tmp "${FEJIX_TESTS}/client/tmp.c")
