#ifndef FEJIX_UTILS_GEOMETRY_H_INCLUDED
#define FEJIX_UTILS_GEOMETRY_H_INCLUDED


#include <fejix/core.h>


/** The length of a metric inch in metres. */
#define FJ_INCH_LENGTH (0.0254)

#define FJ_LEGACY_DPI (96.0)

#define FJ_DPM_INTO_DPI(DPM) ((double) (DPM) * FJ_INCH_LENGTH)

#define FJ_DPM_FROM_DPI(DPI) ((double) (DPI) / FJ_INCH_LENGTH)


/**
    Concepts like text/interface scaling factor are mostly derived from the ratio to 96 DPI.
    That is, if the current DPI is 120, the content of the appropriate size is considered to be
    scaled by 120 / 96 = 125% compared to the "unscaled" ("density-unaware") content rendered at
    96 DPI.
*/
#define FJ_DPM_INTO_SCALING_FACTOR(DPM) (FJ_DPM_INTO_DPI(DPM) / FJ_LEGACY_DPI)

#define FJ_DPM_FROM_SCALING_FACTOR(FACTOR) (FJ_DPM_FROM_DPI(FJ_LEGACY_DPI * (double) (FACTOR)))


typedef double fj_dots_per_metre_t;


typedef uint32_t fj_orientation_t;

enum {
    FJ_ORIENTATION_ROW_RIGHT_COLUMN_DOWN,
    FJ_ORIENTATION_ROW_RIGHT_COLUMN_UP,
    FJ_ORIENTATION_ROW_LEFT_COLUMN_DOWN,
    FJ_ORIENTATION_ROW_LEFT_COLUMN_UP,
    FJ_ORIENTATION_ROW_DOWN_COLUMN_RIGHT,
    FJ_ORIENTATION_ROW_DOWN_COLUMN_LEFT,
    FJ_ORIENTATION_ROW_UP_COLUMN_RIGHT,
    FJ_ORIENTATION_ROW_UP_COLUMN_LEFT,

    FJ_ORIENTATION_STANDARD = FJ_ORIENTATION_ROW_RIGHT_COLUMN_DOWN,
    FJ_ORIENTATION_STANDARD_ROTATED90 = FJ_ORIENTATION_ROW_DOWN_COLUMN_LEFT,
    FJ_ORIENTATION_STANDARD_ROTATED180 = FJ_ORIENTATION_ROW_LEFT_COLUMN_UP,
    FJ_ORIENTATION_STANDARD_ROTATED270 = FJ_ORIENTATION_ROW_UP_COLUMN_RIGHT,
    FJ_ORIENTATION_STANDARD_FLIPPED = FJ_ORIENTATION_ROW_RIGHT_COLUMN_UP,
    FJ_ORIENTATION_STANDARD_FLIPPED_ROTATED90 = FJ_ORIENTATION_ROW_DOWN_COLUMN_RIGHT,
    FJ_ORIENTATION_STANDARD_FLIPPED_ROTATED180 = FJ_ORIENTATION_ROW_LEFT_COLUMN_DOWN,
    FJ_ORIENTATION_STANDARD_FLIPPED_ROTATED270 = FJ_ORIENTATION_ROW_UP_COLUMN_LEFT,
};


struct fj_position {
    uint32_t x;
    uint32_t y;
};

struct fj_offset {
    int32_t x;
    int32_t y;
};

struct fj_size {
    uint32_t width;
    uint32_t height;
};

struct fj_rect {
    struct fj_position position;
    struct fj_size size;
};

struct fj_density {
    fj_dots_per_metre_t row_density;
    fj_dots_per_metre_t column_density;
};


#endif
