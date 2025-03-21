string(REGEX MATCH "^[^_]+" fejix_winapi_win32_version "${fejix_winapi_ntddi_version}")


add_library(fejix_winapi_lib "${fejix_implementation_target_type}")

target_compile_definitions(
    fejix_winapi_lib
    PRIVATE
        "UNICODE"
        "_UNICODE"
        "WIN32_LEAN_AND_MEAN"
        "NTDDI_VERSION=NTDDI_${fejix_winapi_ntddi_version}"
        "_WIN32_WINNT=_WIN32_WINNT_${fejix_winapi_win32_version}")

target_sources(
    fejix_winapi_lib
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/utils.c")

target_link_libraries(
    fejix_winapi_lib
    PRIVATE
        fejix_private_core_lib
    INTERFACE
        fejix_core_lib)


if("${fejix_build_tests}")
    add_executable(fejix_winapi_utils_test_exe "${CMAKE_CURRENT_LIST_DIR}/tests/test-utils.c")
    target_link_libraries(fejix_winapi_utils_test_exe fejix_winapi_lib fejix_private_core_lib)
    add_test(NAME fejix_winapi_utils_test COMMAND fejix_winapi_utils_test_exe)
endif()
