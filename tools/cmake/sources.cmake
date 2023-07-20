target_include_directories(fejix PUBLIC "${FEJIX_INCLUDE}")

target_sources(fejix PRIVATE
    "${FEJIX_SRC}/core/utils.c"
    "${FEJIX_SRC}/core/idlist.c"
    "${FEJIX_SRC}/core/map.c"
)