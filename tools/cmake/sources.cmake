target_include_directories(fejix PUBLIC "${FEJIX_INCLUDE}")


target_sources(fejix PRIVATE
    "${FEJIX_SRC}/core/base.c"
    "${FEJIX_SRC}/core/utils.c"
    "${FEJIX_SRC}/core/list.c"
    "${FEJIX_SRC}/core/map.c"
    "${FEJIX_SRC}/core/sys.c"
)