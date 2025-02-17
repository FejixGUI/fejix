set(fejix_loader_path "${fejix_root_path}/src/loader")

if("${CMAKE_SYSTEM_NAME}" MATCHES "^Windows$|MSYS|Cygwin")
    set(fejix_loader_extra_libs "")
else()
    set(fejix_loader_extra_libs "dl")
endif()


add_library(fejix_loader_lib "${fejix_loader_target_type}")

target_compile_definitions(
    fejix_loader_lib
    INTERFACE
        "FJ_OPT_HAS_LOADER")

target_sources(
    fejix_loader_lib
    PRIVATE
        "${fejix_loader_path}/loader.c"
        "${fejix_loader_path}/autogen/_loader_function_list.c")

target_link_libraries(
    fejix_loader_lib
    PRIVATE
        fejix_private_core_lib
        "${fejix_loader_extra_libs}")

foreach(library IN LISTS fejix_loader_default_libraries)
    target_compile_definitions(
        fejix_loader_lib
        PRIVATE
            "FJ_OPT_LOADER_HAS_$<UPPER_CASE:${library}>")
endforeach()

foreach(interface IN LISTS fejix_interfaces)
    target_sources(
        fejix_loader_lib
        PRIVATE
            "${fejix_loader_path}/autogen/${interface}.c")
endforeach()


install(
    TARGETS fejix_loader_lib
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")
