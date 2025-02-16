set(fejix_core_path "${fejix_root_path}/src/core")

add_library(fejix_core_lib STATIC)

set_target_properties(
    fejix_core_lib
    PROPERTIES
        POSITION_INDEPENDENT_CODE ON)

target_sources(
    fejix_core_lib
    PRIVATE
        "${fejix_core_path}/alloc.c"
        "${fejix_core_path}/base.c"
        "${fejix_core_path}/library.c"
        "${fejix_core_path}/utils.c"
        "${fejix_core_path}/vec.c")

target_compile_definitions(
    fejix_core_lib
    PRIVATE
        "FJ_OPT_PRIVATE")

target_link_libraries(
    fejix_core_lib
    PUBLIC
        fejix_headers_lib)


add_library(fejix_private_core_lib INTERFACE)

target_link_libraries(
    fejix_private_core_lib
    INTERFACE
        fejix_core_lib
        fejix_private_headers_lib)

target_compile_features(
    fejix_private_core_lib
    INTERFACE
        "c_std_99")

target_compile_definitions(
    fejix_private_core_lib
    INTERFACE
        "FJ_OPT_PRIVATE"
        "$<$<C_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>")

target_compile_options(
    fejix_private_core_lib
    INTERFACE
        "$<$<C_COMPILER_ID:MSVC>:/W4>"
        "$<$<C_COMPILER_ID:GNU,Clang,AppleClang>:-fvisibility=hidden;-Werror=vla;-Wall;-Wextra;-Wpedantic>")

if(fejix_build_tests)
    add_executable(fejix_core_vec_test_exe "${fejix_core_path}/tests/test-vec.c")
    target_link_libraries(fejix_core_vec_test_exe PRIVATE fejix_core_lib)
    add_test(NAME fejix_core_vec_test COMMAND fejix_core_vec_test_exe)
endif()
