target_include_directories(fejix PUBLIC "${FEJIX_INCLUDE}")
target_include_directories(fejix PRIVATE "${FEJIX_ROOT}")


# Fejix core
target_sources(
    fejix PRIVATE
    "${FEJIX_SRC}/core/any.c"
    "${FEJIX_SRC}/core/malloc.c"
    "${FEJIX_SRC}/core/vec.c"
    "${FEJIX_SRC}/core/map.c"
    "${FEJIX_SRC}/core/implementation.c"
    "${FEJIX_SRC}/core/ext.c"
)

if(FJ_OPT_NOOP)
    target_sources(
        fejix PRIVATE
        "${FEJIX_SRC}/implementation/noop/implementation.c"
    )
endif()

if(FJ_OPT_WINAPI)
    target_sources(
        fejix PRIVATE
        "${FEJIX_SRC}/implementation/winapi/utils.c"
    )
endif()

if(FJ_OPT_WAYLAND)
    target_sources(
        fejix PRIVATE
        "${FEJIX_SRC}/implementation/wayland/implementation.c"
    )
endif()

if(FJ_OPT_UNIXPOLL)
    target_sources(
        fejix PRIVATE
        "${FEJIX_SRC}/platform/ipc/unixpoll/unixpoll.c"
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
