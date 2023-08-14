if(NOT FEJIX_DEBUG)
    message(FATAL_ERROR "Building tests requires CMAKE_BUILD_TYPE=\"Debug\"")
endif()

enable_testing()

fejix_add_test(fejix_test_core_map "${FEJIX_TESTS}/core/map.c")
fejix_add_test(fejix_test_core_list "${FEJIX_TESTS}/core/list.c")
