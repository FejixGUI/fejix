#ifndef FEJIX_CORE_H_INCLUDED
#define FEJIX_CORE_H_INCLUDED


#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
#    define FJ_PUBLIC_EXTERN_C extern "C"
#else
#    define FJ_PUBLIC_EXTERN_C
#endif

#if defined(_WIN32) && defined(FJ_BUILDING_PRIVATE_CODE)
#    define FJ_PUBLIC_EXPORT __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#    define FJ_PUBLIC_EXPORT __attribute__((visibility("default")))
#else
#    define FJ_PUBLIC_EXPORT
#endif

#define FJ_PUBLIC FJ_PUBLIC_EXTERN_C FJ_PUBLIC_EXPORT


#ifdef FJ_BUILDING_PRIVATE_CODE
#    define FJ_DEFINE_MODULE(MODULE_NAME) struct MODULE_NAME;
#else
#    define FJ_DEFINE_MODULE(MODULE_NAME)                \
        struct MODULE_NAME {                             \
            struct MODULE_NAME##_methods const *methods; \
        };
#endif

#ifdef FJ_BUILDING_PRIVATE_CODE
#    define FJ_DEFINE_OBJECT(OBJECT_NAME) struct OBJECT_NAME;
#else
#    define FJ_DEFINE_OBJECT(OBJECT_NAME) \
        struct OBJECT_NAME {              \
            void *userdata                \
        };
#endif

#define FJ_TIME_FROM_NANOS(NANOSECONDS) ((fj_time) (NANOSECONDS))
#define FJ_TIME_FROM_MICROS(MICROSECONDS) ((fj_time) (MICROSECONDS) * UINT64_C(1000))
#define FJ_TIME_FROM_MILLIS(MILLISECONDS) ((fj_time) (MILLISECONDS) * UINT64_C(1000000))
#define FJ_TIME_FROM_SECONDS(SECONDS) ((fj_time) (SECONDS) * UINT64_C(1000000000))
#define FJ_TIME_INTO_NANOS(TIME) (TIME)
#define FJ_TIME_INTO_MICROS(TIME) ((TIME) / UINT64_C(1000))
#define FJ_TIME_INTO_MILLIS(TIME) ((TIME) / UINT64_C(1000000))
#define FJ_TIME_INTO_SECONDS(TIME) ((TIME) / UINT64_C(1000000000))

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


#define FJ_TRY(EXPR)                                                                          \
    for (fj_err fj_result = (EXPR), _fj_try_guard = 1; _fj_try_guard == 1; _fj_try_guard = 0) \
        if (fj_result != FJ_OK)  // Outside of for loop to avoid dead code if the block returns


/** Error code. */
typedef enum {
    /** Success */
    FJ_OK = 0,

    /** Out of memory */
    FJ_ERR_OUT_OF_MEMORY = 1,

    /** The requested operation is not implemented and therefore no work has been done. */
    FJ_ERR_UNIMPLEMENTED = 2,

    /** The operation has failed, a generic error returned when concrete reasons are unknown. */
    FJ_ERR_OPERATION_FAILED = 3,

    /** Input/output operation failed. */
    FJ_ERR_IO_FAILED = 4,

    /** The requested operation or resource are not available on the system. */
    FJ_ERR_UNAVAILABLE = 5,

    /** Access denied to create a file, share memory, connect to a device etc. */
    FJ_ERR_ACCESS_DENIED = 6,

    /** Concurrent access to the object is not permitted. */
    FJ_ERR_CONCURRENT_ACCESS = 7,

    /** Invalid usage indicates a programming error like zero allocation size, index out of range,
        removing from an empty vector etc. */
    FJ_ERR_INVALID_USAGE = 8,

    /** The requested operation cannot be done on the specified object. */
    FJ_ERR_INVALID_OPERATION = 9,

    /** Invalid text encoding. */
    FJ_ERR_INVALID_ENCODING = 10,

    FJ_ERR_ENUM32 = INT32_MAX,
} fj_err;


enum fj_orientation {
    /**
        Rows: address-increasing memory direction of pixels corresponds to RIGHT.
        Columns: address-increasing memory direction of rows corresponds to DOWN.
    */
    FJ_ORIENTATION_ROW_RIGHT_COLUMN_DOWN,
    /** */
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

    FJ_ORIENTATION_ENUM32 = INT32_MAX,
};


/** Time in nanoseconds. */
typedef uint64_t fj_time;

/** Dots-per-metres (DPM). */
typedef double fj_density;


struct fj_version {
    uint8_t major;
    uint8_t minor;
    uint8_t patch;
};

/** 2D absolute position. */
struct fj_position2d {
    /** X coordinate. */
    uint32_t x;
    /** Y coordinate. */
    uint32_t y;
};

/** 2D relative offset. */
struct fj_offset2d {
    /** X coordinate. */
    int32_t x;
    /** Y coordinate. */
    int32_t y;
};

/** 2D size, can be zero. */
struct fj_size2d {
    /** Width. */
    uint32_t width;
    /** Height */
    uint32_t height;
};

/** Represents a real rectangular part of some visible area. */
struct fj_rect2d {
    /** Absolute position */
    struct fj_position2d position;

    /** Rectangle size */
    struct fj_size2d size;
};

/** Represents a rectangle in coordinates relative to some visible area. */
struct fj_viewport2d {
    /** Relative position */
    struct fj_offset2d offset;

    /** Viewport size */
    struct fj_size2d size;
};


/** Always returns a valid printable string, even for invalid error IDs. */
FJ_PUBLIC
char const *fj_err_get_description(fj_err error);


#endif
