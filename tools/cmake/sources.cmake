target_include_directories(fejix PUBLIC "${FEJIX_INCLUDE}")
target_include_directories(fejix PRIVATE "${FEJIX_ROOT}")


# Fejix core
target_sources(
    fejix PRIVATE
    "${FEJIX_SRC}/core/malloc.c"
    "${FEJIX_SRC}/core/utils.c"
    "${FEJIX_SRC}/core/protocol.c"
    "${FEJIX_SRC}/core/helper.c"
)


if(FJ_OPT_WINAPI)
    target_sources(
        fejix PRIVATE
        "${FEJIX_SRC}/winapi/protocol.c"
        "${FEJIX_SRC}/winapi/utils.c"
    )
endif()


# Define "FJ_FILENAME" for every source file
get_target_property(fejix_source_files fejix SOURCES)
foreach(file ${fejix_source_files})
    file(RELATIVE_PATH filename "${FEJIX_ROOT}" "${file}")

    set_property(
        SOURCE "${file}"
        APPEND PROPERTY COMPILE_DEFINITIONS "FJ_FILENAME=\"${filename}\""
    )
endforeach()
