enable_testing()

macro(fejix_add_test test_name source_file)
    add_executable(${test_name} "${source_file}")
    target_link_libraries(${test_name} fejix)
    add_test(NAME ${test_name} COMMAND ${test_name})
endmacro()


fejix_add_test(fejix_test_core_map "${FEJIX_TESTS}/core/map.c")
fejix_add_test(fejix_test_core_list "${FEJIX_TESTS}/core/list.c")
fejix_add_test(fejix_test_core_sys "${FEJIX_TESTS}/core/sys.c")