target_include_directories(fejix PUBLIC "${FEJIX_INCLUDE}")
target_include_directories(fejix PRIVATE "${FEJIX_ROOT}")

target_sources(fejix PRIVATE
    "${FEJIX_SRC}/core/malloc.c"
    "${FEJIX_SRC}/core/list.c"
    "${FEJIX_SRC}/core/map.c"
    "${FEJIX_SRC}/modules/client/client.c"
)

if(FEJIX_PLATFORM_X11)
    target_sources(fejix PRIVATE
        "${FEJIX_SRC}/modules/x11/client.c"
    )
endif()


# Define "FJ_FILENAME" for every source file
get_target_property(fejix_source_files fejix SOURCES)
foreach(file ${fejix_source_files})
    file(RELATIVE_PATH filename "${FEJIX_SRC}" "${file}")
    
    set_property(
        SOURCE "${file}"
        APPEND PROPERTY COMPILE_DEFINITIONS
        "FJ_FILENAME=\"${filename}\""
    )
endforeach()
