string(REGEX MATCH "^[^_]+" fejix_winapi_winnt_version "${fejix_winapi_target_version}")

# It is intended that you #undef WIN32_LEAN_AND_MEAN every time you need something sophisticated
target_compile_definitions(
    fejix
    PRIVATE
        "UNICODE"
        "_UNICODE"
        "WIN32_LEAN_AND_MEAN"
        "NTDDI_VERSION=NTDDI_${fejix_winapi_target_version}"
        "_WIN32_WINNT=_WIN32_WINNT_${fejix_winapi_winnt_version}")

target_sources(
    fejix
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/utils.c")


if("${fejix_build_tests}")
    add_executable(fejix_winapi_utils_test_exe "${CMAKE_CURRENT_LIST_DIR}/tests/test-utils.c")
    target_link_libraries(fejix_winapi_utils_test_exe fejix)
    add_test(NAME fejix_winapi_utils_test COMMAND fejix_winapi_utils_test_exe)
endif()
