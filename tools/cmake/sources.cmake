target_include_directories(fejix PUBLIC "${FEJIX_INCLUDE}")

target_sources(fejix PRIVATE
    "${FEJIX_SRC}/core/malloc.c"
    "${FEJIX_SRC}/core/list.c"
    "${FEJIX_SRC}/core/map.c"
)


fejix_define_filename_for_sources(fejix "FJ_FILENAME")
