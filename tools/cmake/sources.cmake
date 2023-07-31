target_include_directories(fejix PUBLIC "${FEJIX_INCLUDE}")

set_target_properties(fejix PROPERTIES C_STANDARD 99)

target_sources(fejix PRIVATE
    "${FEJIX_SRC}/core/base.c"
    "${FEJIX_SRC}/core/utils.c"
    "${FEJIX_SRC}/core/list.c"
    "${FEJIX_SRC}/core/map.c"
    "${FEJIX_SRC}/core/sys.c"
)