set(fejix_loader_path "${fejix_root_path}/src/loader")

if("${CMAKE_SYSTEM_NAME}" MATCHES "^Windows$|MSYS|Cygwin")
    set(fejix_loader_extra_libs "")
else()
    set(fejix_loader_extra_libs "dl")
endif()


add_library(fejix_loader_lib "${fejix_loader_target_type}")

target_sources(
    fejix_loader_lib
    PRIVATE
        "${fejix_loader_path}/loader.c")

target_link_libraries(
    fejix_loader_lib
    PRIVATE
        fejix_private_core_lib
        "${fejix_loader_extra_libs}")

foreach(interface IN LISTS fejix_interfaces)
    target_sources(
        fejix_loader_lib
        PRIVATE
            "${fejix_loader_path}/autogen/${interface}.c")
endforeach()


install(
    TARGETS fejix_loader_lib
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")
