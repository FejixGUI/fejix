target_sources(
    fejix
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/logging.c"
        "${CMAKE_CURRENT_LIST_DIR}/math.c"
        "${CMAKE_CURRENT_LIST_DIR}/memory.c"
        "${CMAKE_CURRENT_LIST_DIR}/vector.c")

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    target_compile_definitions(
        fejix
        PRIVATE
            "FJ_COMPILE_OPT_ENABLE_LOGGING"
            "FJ_COMPILE_OPT_ENABLE_WARN"
            "FJ_COMPILE_OPT_ENABLE_INFO"
    )
endif()

if("${fejix_build_tests}")
    add_executable(fejix_core_vec_test_exe "${CMAKE_CURRENT_LIST_DIR}/tests/test-vec.c")
    target_link_libraries(fejix_core_vec_test_exe PRIVATE fejix fejix_private_headers)
    add_test(NAME fejix_core_vec_test COMMAND fejix_core_vec_test_exe)
endif()
