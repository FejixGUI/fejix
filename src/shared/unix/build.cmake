target_sources(
    fejix
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/events.c"
        "${CMAKE_CURRENT_LIST_DIR}/shared_memory.c")

if("${fejix_build_tests}")
    add_executable(
        fejix_unix_shared_memory_test_exe
            "${CMAKE_CURRENT_LIST_DIR}/tests/test-shared_memory.c")

    target_link_libraries(fejix_unix_shared_memory_test_exe
        PRIVATE
            fejix fejix_private_headers)

    add_test(
        NAME fejix_unix_shared_memory_test
        COMMAND fejix_unix_shared_memory_test_exe)
endif()
