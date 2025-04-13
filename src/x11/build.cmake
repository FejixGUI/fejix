set(fejix_build_unix_utils ON)

target_sources(
    fejix
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/modules.c"
        "${CMAKE_CURRENT_LIST_DIR}/app/app.c")

target_link_libraries(
    fejix
    PRIVATE
        "X11"
        "xcb"
        "X11-xcb")
