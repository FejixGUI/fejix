string(REGEX MATCH "^[^_]+" fejix_winapi_winnt_version "${fejix_winapi_target_version}")

# It is intended that you #undef WIN32_LEAN_AND_MEAN every time you need something sophisticated
target_compile_definitions(
    fejix
    PRIVATE
        "FJ_OPT_WINAPI"
        "UNICODE"
        "_UNICODE"
        "WIN32_LEAN_AND_MEAN"
        "NTDDI_VERSION=NTDDI_${fejix_winapi_target_version}"
        "_WIN32_WINNT=_WIN32_WINNT_${fejix_winapi_winnt_version}")

target_sources(
    fejix
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/utils/encoding.c"
        "${CMAKE_CURRENT_LIST_DIR}/utils/window.c"
        "${CMAKE_CURRENT_LIST_DIR}/io_thread/io_thread.c"
        "${CMAKE_CURRENT_LIST_DIR}/io_thread/timer.c"
        "${CMAKE_CURRENT_LIST_DIR}/window/window.c"
        "${CMAKE_CURRENT_LIST_DIR}/window/view.c")


if("${fejix_tests}")
    add_executable(fejix_winapi_utils_test_exe "${CMAKE_CURRENT_LIST_DIR}/tests/test-utils.c")
    target_link_libraries(fejix_winapi_utils_test_exe fejix fejix_private_headers)
    add_test(NAME fejix_winapi_utils_test COMMAND fejix_winapi_utils_test_exe)
endif()
