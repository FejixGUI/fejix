macro(fejix_add_optional_definition name)
    option(FEJIX_${name} OFF)

    if (FEJIX_${name})
        add_compile_definitions("FJ_${name}")
    endif()
endmacro()


macro(fejix_add_test test_name source_file)
    add_executable(${test_name} "${source_file}")
    target_link_libraries(${test_name} fejix)
    add_test(NAME ${test_name} COMMAND ${test_name})
endmacro()
