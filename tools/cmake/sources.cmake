target_include_directories(fejix PUBLIC "${FEJIX_INCLUDE}")
target_include_directories(fejix PRIVATE "${FEJIX_ROOT}")

# Suppress pointless warnings from MSVC 
add_compile_definitions("_CRT_SECURE_NO_WARNINGS")

target_sources(fejix PRIVATE
    "${FEJIX_SRC}/core/malloc.c"
    "${FEJIX_SRC}/core/utils.c"
    "${FEJIX_SRC}/core/list.c"
    "${FEJIX_SRC}/client.c"
)

if(FEJIX_PLATFORM_X11)
    target_sources(fejix PRIVATE
        "${FEJIX_SRC}/modules/x11/client.c"
    )
endif()

if(FEJIX_FEATURE_UNIXPOLLER)
    target_sources(fejix PRIVATE
        "${FEJIX_SRC}/modules/unixpoller/unixpoller.c"
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
