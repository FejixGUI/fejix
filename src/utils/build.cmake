target_sources(
    fejix
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/math.c"
        "${CMAKE_CURRENT_LIST_DIR}/memory.c"
        "${CMAKE_CURRENT_LIST_DIR}/vector.c")

if("${fejix_build_tests}")
    add_executable(fejix_core_vec_test_exe "${CMAKE_CURRENT_LIST_DIR}/tests/test-vec.c")
    target_link_libraries(fejix_core_vec_test_exe PRIVATE fejix)
    add_test(NAME fejix_core_vec_test COMMAND fejix_core_vec_test_exe)
endif()
