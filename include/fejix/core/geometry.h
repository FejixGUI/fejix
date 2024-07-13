#ifndef FEJIX_CORE_GEOMETRY_H_
#define FEJIX_CORE_GEOMETRY_H_


#include <fejix/core/base.h>


void fj_position2d_get_bounding_rect(
    struct fj_position2d const * position1,
    struct fj_position2d const * position2,
    struct fj_rect2d /*out*/ * bounding_rect
);


fj_bool32_t fj_rect2d_is_empty(
    struct fj_rect2d const * rect
);


fj_bool32_t fj_region_is_empty(
    struct fj_rect2d const */*[]?*/ rects,
    uint32_t rect_count
);

void fj_region_get_bounding_rect(
    struct fj_rect2d const */*[]?*/ rects,
    uint32_t rect_count,
    struct fj_rect2d /*out*/ * bounding_rect
);


#endif
