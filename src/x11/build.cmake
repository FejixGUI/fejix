set(fejix_build_unix_utils ON)

target_compile_definitions(
    fejix
    PRIVATE
        "FJ_COMPILE_OPT_ENABLE_X11_BACKEND")

target_sources(
    fejix
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/backend.c"
        "${CMAKE_CURRENT_LIST_DIR}/app.c"
        "${CMAKE_CURRENT_LIST_DIR}/window.c")

target_link_libraries(
    fejix
    PRIVATE
        "X11"
        "xcb"
        "X11-xcb")
