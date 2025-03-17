#ifndef FEJIX_CORE_H_INCLUDED
#define FEJIX_CORE_H_INCLUDED


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
#    define FJ_PUBLIC_C_LINKAGE extern "C"
#else
#    define FJ_PUBLIC_C_LINKAGE
#endif

#if defined(_WIN32) && defined(FJ_OPT_PUBLIC_EXPORT)
#    define FJ_PUBLIC_VISIBILITY __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#    define FJ_PUBLIC_VISIBILITY __attribute__((visibility("default")))
#else
#    define FJ_PUBLIC_VISIBILITY
#endif

#define FJ_PUBLIC FJ_PUBLIC_C_LINKAGE FJ_PUBLIC_VISIBILITY

#define FJ_PUBLIC_INLINE FJ_PUBLIC_C_LINKAGE static inline


#define FJ_VERSION(MAJOR, MINOR, PATCH)                                         \
    ((fj_version_t) ((((MAJOR) & 0xfffff) << 20) | (((MINOR) & 0xfffff) << 10)) \
     | ((PATCH) & 0xfffff))

#define FJ_VERSION_MAJOR(VERSION) ((VERSION) >> 20 & 0xfffff)
#define FJ_VERSION_MINOR(VERSION) ((VERSION) >> 10 & 0xfffff)
#define FJ_VERSION_PATCH(VERSION) ((VERSION) & 0xfffff)

/** Gets the next major version. The next version for `1.2.3` is `2.0.0`. */
#define FJ_VERSION_NEXT(VERSION) (FJ_VERSION(FJ_VERSION_MAJOR(VERSION) + 1, 0, 0))

/** Checks if the version is compatible with the required version.
    For example, version `1.2.3` is compatible with versions from `1.2.3` to `2.0.0`. */
#define FJ_VERSION_COMPATIBLE(VERSION, WITH_VERSION) \
    ((VERSION) >= WITH_VERSION && (VERSION) < FJ_VERSION_NEXT(WITH_VERSION))


#define FJ_TIME_FROM_MICROS(MICROSECONDS) ((fj_time_t) (MICROSECONDS))
#define FJ_TIME_FROM_MILLIS(MILLISECONDS) ((fj_time_t) (MILLISECONDS) * UINT64_C(1000))
#define FJ_TIME_FROM_SECONDS(SECONDS) ((fj_time_t) (MILLISECONDS) * UINT64_C(1000000))
#define FJ_TIME_INTO_MICROS(TIME) (TIME)
#define FJ_TIME_INTO_MILLIS(TIME) ((TIME) / UINT64_C(1000))
#define FJ_TIME_INTO_SECONDS(TIME) ((TIME) / UINT64_C(1000000))

/** The length of a metric inch in metres. */
#define FJ_INCH_LENGTH (0.0254)
#define FJ_LEGACY_DPI (96.0)

#define FJ_DENSITY_INTO_DPM(DENSITY) (DENSITY)
#define FJ_DENSITY_INTO_DPI(DENSITY) ((double) (DENSITY) * FJ_INCH_LENGTH)
#define FJ_DENSITY_FROM_DPM(DPM) (DPM)
#define FJ_DENSITY_FROM_DPI(DPI) ((double) (DPI) / FJ_INCH_LENGTH)

/** Concepts like text/interface scaling factor are mostly derived from the ratio to 96 DPI.
    That is, if the current DPI is 120, the content of the appropriate size is considered to be
    scaled by 120 / 96 = 125% compared to the "unscaled" ("density-unaware") content rendered at
    96 DPI. */
#define FJ_DPM_INTO_LEGACY_SCALING(DENSITY) (FJ_DENSITY_INTO_DPI(DENSITY) / FJ_LEGACY_DPI)
#define FJ_DPM_FROM_LEGACY_SCALING(FACTOR) (FJ_DENSITY_FROM_DPI(FJ_LEGACY_DPI * (double) (FACTOR)))


#define FJ_TRY(EXPR)                                                                            \
    for (fj_err_t fj_result = (EXPR), _fj_try_guard = 1; _fj_try_guard == 1; _fj_try_guard = 0) \
        if (fj_result != FJ_OK)  // Outside of for loop to avoid dead code if the block returns


/** Error code. */
typedef uint32_t fj_err_t;

enum {
    FJ_OK,

    FJ_ERR_OUT_OF_MEMORY,

    /** The requested operation is not implemented and therefore no work has been done. */
    FJ_ERR_UNIMPLEMENTED,

    /** The operation has failed, a generic error returned when concrete reasons are unknown. */
    FJ_ERR_OPERATION_FAILED,

    /** Input/output operation failed. */
    FJ_ERR_IO_FAILED,

    /** The requested operation or resource are not available on the system. */
    FJ_ERR_UNAVAILABLE,

    /** Access denied to create a file, share memory, connect to a device etc. */
    FJ_ERR_ACCESS_DENIED,

    /** Concurrent access to the object is not permitted. */
    FJ_ERR_CONCURRENT_ACCESS,

    /** Invalid usage indicates a programming error like zero allocation size, index out of range,
        removing from an empty vector etc. */
    FJ_ERR_INVALID_USAGE,

    /** The requested operation cannot be done on the specified object. */
    FJ_ERR_INVALID_OPERATION,

    /** Invalid text encoding. */
    FJ_ERR_INVALID_ENCODING,
};


typedef uint32_t fj_orientation_t;

enum {
    /**
        Rows: address-increasing memory direction of pixels corresponds to RIGHT.
        Columns: address-increasing memory direction of rows corresponds to DOWN.
    */
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


/** Version triple. */
typedef uint32_t fj_version_t;

/** Time in microseconds. */
typedef uint64_t fj_time_t;

/** Dots-per-metres (DPM). */
typedef double fj_density_t;

/** The values are defined in `utils/color_formats.h` */
typedef uint32_t fj_color_format_t;


typedef struct fj_position {
    uint32_t x;
    uint32_t y;
} fj_position_t;

typedef struct fj_offset {
    int32_t x;
    int32_t y;
} fj_offset_t;

typedef struct fj_size {
    uint32_t width;
    uint32_t height;
} fj_size_t;

typedef struct fj_rect {
    struct fj_position position;
    struct fj_size size;
} fj_rect_t;

typedef struct fj_viewport {
    struct fj_offset offset;
    struct fj_size size;
} fj_viewport_t;


/** Always returns a valid printable string, even for invalid error IDs. */
FJ_PUBLIC
char const *fj_err_get_description(fj_err_t error);


#endif
