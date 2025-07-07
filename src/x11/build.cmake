set(fejix_build_unix_utils ON)

target_compile_definitions(
    fejix
    PRIVATE
        "FJ_COMPILE_OPT_ENABLE_X11")

target_sources(
    fejix
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/platform.c"

        "${CMAKE_CURRENT_LIST_DIR}/app.c")

target_link_libraries(
    fejix
    PRIVATE
        "X11"
        "X11-xcb"
        "xcb")
