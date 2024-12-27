#ifndef FEJIX_CORE_GEOMETRY_H_
#define FEJIX_CORE_GEOMETRY_H_


#include <fejix/core/base.h>


typedef uint32_t fj_orientation_type_t;

enum fj_orientation_type {
    FJ_ORIENTATION_NORMAL = 0,
    /** Vertical flip (along the Y axis). */
    FJ_ORIENTATION_VFLIP = (1 << 0),
    /** Horizontal flip (along the X axis). */
    FJ_ORIENTATION_HFLIP = (1 << 1),
    /** Clock-wise 90 degree rotation. */
    FJ_ORIENTATION_ROTATE90 = (1 << 2),

    FJ_ORIENTATION_ROTATE180 = FJ_ORIENTATION_HFLIP ^ FJ_ORIENTATION_VFLIP,
    FJ_ORIENTATION_ROTATE270 = FJ_ORIENTATION_ROTATE180 ^ FJ_ORIENTATION_ROTATE90,
    FJ_ORIENTATION_ROTATE90_VFLIP = FJ_ORIENTATION_ROTATE90 ^ FJ_ORIENTATION_VFLIP,
    FJ_ORIENTATION_ROTATE180_VFLIP = FJ_ORIENTATION_ROTATE180 ^ FJ_ORIENTATION_VFLIP,
    FJ_ORIENTATION_ROTATE270_VFLIP = FJ_ORIENTATION_ROTATE270 ^ FJ_ORIENTATION_VFLIP,
};


struct fj_position2d {
    uint32_t x;
    uint32_t y;
};

struct fj_offset2d {
    int32_t x;
    int32_t y;
};

struct fj_size2d {
    uint32_t width;
    uint32_t height;
};

struct fj_density2d {
    /** Number of pixels per ``unit_length``. */
    uint32_t vertical;
    /** Number of pixels per ``unit_length``. */
    uint32_t horizontal;

    /** The length of the unit in metres. */
    uint32_t unit_length;
};

struct fj_rect2d {
    struct fj_position2d position;
    struct fj_size2d size;
};

struct fj_viewport2d {
    struct fj_offset2d offset;
    struct fj_size2d size;
};


#endif
