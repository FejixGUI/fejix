set(fejix_headers_path "${fejix_root_path}/src/headers")

set(
    fejix_interfaces
    "app_clock"
    "app"
    "image"
    "implementation"
    "opengl"
    "ram"
    "view")


add_library(fejix_headers_lib INTERFACE)

target_include_directories(
    fejix_headers_lib
    INTERFACE
        "${fejix_headers_path}")


add_library(fejix_private_headers_lib INTERFACE)

target_include_directories(
    fejix_private_headers_lib
    INTERFACE
        "${fejix_root_path}")

target_link_libraries(
    fejix_private_headers_lib
    INTERFACE
        fejix_headers_lib)


install(
    DIRECTORY "${fejix_headers_path}/fejix"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    FILES_MATCHING PATTERN "*.h"
    PATTERN ".*" EXCLUDE)
