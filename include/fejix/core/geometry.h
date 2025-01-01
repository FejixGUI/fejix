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


struct fj_position {
    uint32_t x;
    uint32_t y;
};

struct fj_relative_position {
    int32_t x;
    int32_t y;
};

struct fj_size {
    uint32_t width;
    uint32_t height;
};

struct fj_density {
    /** Number of pixels per ``physical_unit_length``. */
    uint32_t vertical_pixels_per_unit;
    /** Number of pixels per ``physical_unit_length``. */
    uint32_t horizontal_pixels_per_unit;

    /** The length of the unit in metres. */
    uint32_t physical_unit_length;
};

struct fj_rect {
    struct fj_position position;
    struct fj_size size;
};

struct fj_relative_rect {
    struct fj_relative_position relative_position;
    struct fj_size size;
};


#endif
