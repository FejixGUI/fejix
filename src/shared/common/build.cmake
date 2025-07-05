target_sources(
    fejix
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/error.c"
        "${CMAKE_CURRENT_LIST_DIR}/list.c"
        "${CMAKE_CURRENT_LIST_DIR}/math.c"
        "${CMAKE_CURRENT_LIST_DIR}/memory.c")

if("${fejix_enable_errors}")
    target_compile_definitions(
        fejix
        PRIVATE
            "FJ_COMPILE_OPT_ENABLE_ERRORS")
endif()

if("${fejix_enable_error_locations}")
    target_compile_definitions(
        fejix
        PRIVATE
            "FJ_COMPILE_OPT_ENABLE_ERROR_LOCATIONS")
endif()

if("${fejix_build_tests}")
    add_executable(
        fejix_core_list_test
        "${CMAKE_CURRENT_LIST_DIR}/tests/test-list.c")

    target_link_libraries(
        fejix_core_list_test
        PRIVATE
            fejix
            fejix_private_headers)

    add_test(
        NAME fejix_core_list_test
        COMMAND fejix_core_list_test)
endif()
