#ifndef FEJIX_CORE_IMAGE_INCLUDED
#define FEJIX_CORE_IMAGE_INCLUDED


#include <fejix/core/base.h>


/** The length of a metric inch in metres. */
#define FJ_INCH_LENGTH (0.0025)

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

#define FJ_DPM_FROM_SCALING_FACTOR(FACTOR) (FJ_DPM_FROM_DPI(96.0 * (double) (FACTOR)))


/**
    Pixel density in DPM (dots-per-metre).

    To calculate DPI (dots-per-inch), use ``FJ_DPM_INTO_DPI``.

    To calculate scaling factor relative to the legacy 96 DPI, use ``FJ_DPM_INTO_SCALING_FACTOR``.
*/
typedef double fj_dots_per_metre_t;


typedef uint64_t fj_microseconds_t;


typedef uint32_t fj_orientation_id_t;

enum {
    FJ_ORIENTATION_NORMAL = 1,
    FJ_ORIENTATION_ROTATE90 = 1 << 1,
    FJ_ORIENTATION_ROTATE180 = 1 << 2,
    FJ_ORIENTATION_ROTATE270 = 1 << 3,
    FJ_ORIENTATION_ROTATE90_VFLIP = 1 << 4,
    FJ_ORIENTATION_ROTATE180_VFLIP = 1 << 5,
    FJ_ORIENTATION_ROTATE270_VFLIP = 1 << 6,
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

struct fj_rect {
    struct fj_position position;
    struct fj_size size;
};

struct fj_relative_rect {
    struct fj_relative_position relative_position;
    struct fj_size size;
};

struct fj_density {
    fj_dots_per_metre_t row_density;
    fj_dots_per_metre_t column_density;
};

struct fj_geometry {
    struct fj_size size;
    struct fj_density density;
    fj_orientation_id_t orientation;
};


#endif
