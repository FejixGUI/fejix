target_include_directories(fejix PUBLIC "${FEJIX_INCLUDE}")
target_include_directories(fejix PRIVATE "${FEJIX_ROOT}")


target_sources(fejix PRIVATE
    "${FEJIX_SRC}/core/malloc.c"
    "${FEJIX_SRC}/core/utils.c"
    "${FEJIX_SRC}/core/list.c"
    "${FEJIX_SRC}/core/client.c"
)

if(FEJIX_PLATFORM_X11)
    target_sources(fejix PRIVATE
        "${FEJIX_SRC}/x11/client.c"
    )
endif()

if(FEJIX_PLATFORM_WINAPI)
    target_sources(fejix PRIVATE
        "${FEJIX_SRC}/winapi/client.c"
        "${FEJIX_SRC}/winapi/utils.c"
    )
endif()

if(FEJIX_FEATURE_UNIXPOLLER)
    target_sources(fejix PRIVATE
        "${FEJIX_SRC}/unixpoller/unixpoller.c"
    )
endif()


# Define "FJ_FILENAME" for every source file
get_target_property(fejix_source_files fejix SOURCES)
foreach(file ${fejix_source_files})
    file(RELATIVE_PATH filename "${FEJIX_ROOT}" "${file}")

    set_property(
        SOURCE "${file}"
        APPEND PROPERTY COMPILE_DEFINITIONS
        "FJ_FILENAME=\"${filename}\""
    )
endforeach()
