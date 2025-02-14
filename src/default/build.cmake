set(fejix_default_source_path "${fejix_root_path}/src/default")

# Usage:
# set(excluded_sources "app" "app_clock")
# fejix_add_default_sources(my_implementation_library "${excluded_sources}")
function(fejix_add_default_sources _target _excluded_sources)
    foreach(interface IN LISTS fejix_interfaces)
        if("${interface}" IN_LIST _excluded_sources)
            continue()
        endif()

        target_sources("${_target}" "${fejix_default_source_path}/autogen/${interface}.c")
    endforeach()
endfunction()
