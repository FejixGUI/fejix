/**
    \file
*/

#ifndef FEJIX_BASE_H_
#define FEJIX_BASE_H_


#include <fejix/version.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


#if defined(FJ_COMPILE_OPT_DOCS)

/**
    Defines the appropriate external linkage.

    This expands to `extern` (`extern "C"` for C++) and adds special attributes for shared libraries
    if needed.

    If you are using the library as shared (`.dll`) on Microsoft Windows, you need to define
    `FJ_INCLUDE_OPT_DLLIMPORT` before including any library headers so that the library gets linked
    correctly.
*/
#    define FJ_PUBLIC extern

#else

#    if defined(__cplusplus)
#        define FJ_PUBLIC_LINKAGE extern "C"
#    else
#        define FJ_PUBLIC_LINKAGE extern
#    endif

#    if defined(FJ_COMPILE_OPT_DLLEXPORT)
#        define FJ_PUBLIC_VISIBILITY __declspec(dllexport)
#    elif defined(FJ_INCLUDE_OPT_DLLIMPORT)
#        define FJ_PUBLIC_VISIBILITY __declspec(dllimport)
#    elif defined(__GNUC__) && __GNUC__ >= 4
#        define FJ_PUBLIC_VISIBILITY __attribute__((visibility("default")))
#    else
#        define FJ_PUBLIC_VISIBILITY
#    endif

#    define FJ_PUBLIC FJ_PUBLIC_LINKAGE FJ_PUBLIC_VISIBILITY

#endif

#if defined(FJ_COMPILE_OPT_PRIVATE_CODE)
#    define FJ_HANDLE(TYPE) struct TYPE;
#else
/**
    Forward-declares a struct.

    Even though each backend may define the struct in its own way, the first field is always the
    userdata pointer.
*/
#    define FJ_HANDLE(TYPE) \
        struct TYPE {       \
            void *userdata; \
        };
#endif

#if defined(FJ_INTERNAL_OPT_DEFINE_DEFAULT_API_IMPLEMENTATIONS)
#    define FJ_API_EX(FUNCTION_NAME, RETURN_TYPE, RETURN_VALUE, ...) \
        static RETURN_TYPE FUNCTION_NAME##_default(__VA_ARGS__)      \
        {                                                            \
            return RETURN_VALUE;                                     \
        }                                                            \
        FJ_PUBLIC RETURN_TYPE (*FUNCTION_NAME)(__VA_ARGS__);         \
        RETURN_TYPE (*FUNCTION_NAME)(__VA_ARGS__) = FUNCTION_NAME##_default;
#else
/** Defines a public API function with explicit return type and default return value. */
#    define FJ_API_EX(FUNCTION_NAME, RETURN_TYPE, RETURN_VALUE, ...) \
        FJ_PUBLIC RETURN_TYPE (*FUNCTION_NAME)(__VA_ARGS__);
#endif

/** Defines a public API function that returns fj_error and defaults to #FJ_ERROR_UNIMPLEMENTED. */
#define FJ_API(FUNCTION_NAME, ...) \
    FJ_API_EX(FUNCTION_NAME, enum fj_error, FJ_ERROR_UNIMPLEMENTED, __VA_ARGS__)

/** Defines a public API function that returns void and defaults to no-op. */
#define FJ_API_VOID(FUNCTION_NAME, ...) FJ_API_EX(FUNCTION_NAME, void, , __VA_ARGS__)


#if defined(FJ_COMPILE_OPT_PRIVATE_CODE)
#    define FJ_API_INIT(FUNCTION_NAME) FUNCTION_NAME = FUNCTION_NAME##_;
#endif


/** Status code. */
enum fj_error {
    /** Success */
    FJ_OK,

    /** The requested operation is not implemented and therefore no work has been done. */
    FJ_ERROR_UNIMPLEMENTED,

    /** Out of memory */
    FJ_ERROR_OUT_OF_MEMORY,

    /** Input/output operation failed. */
    FJ_ERROR_IO_FAILED,

    /** The requested operation or resource are not available on the system. */
    FJ_ERROR_UNAVAILABLE,

    /** Access denied to create a file, share memory, connect to a device etc. */
    FJ_ERROR_ACCESS_DENIED,

    /** Concurrent access to the object is not permitted. */
    FJ_ERROR_CONCURRENT_ACCESS,

    /** Invalid usage indicates a programming error like zero allocation size, index out of range,
        removing from an empty vector etc. */
    FJ_ERROR_INVALID_USAGE,

    /** The requested operation cannot be done on the specified object. */
    FJ_ERROR_INVALID_OPERATION,

    /** Invalid text encoding. */
    FJ_ERROR_INVALID_ENCODING,

    /** The operation has failed, a generic error returned when concrete reasons are unknown. */
    FJ_ERROR_OPERATION_FAILED,

    FJ_ERROR_MAX,

    FJ_ERROR_ENUM32 = INT32_MAX,
};


enum fj_orientation {
    /**
        The standard orientation implies that:
        - memory-increasing direction of pixels corresponds to RIGHT
        - memory-increasing direction of rows corresponds to DOWN
    */
    FJ_ORIENTATION_STANDARD,
    FJ_ORIENTATION_ROTATED90,
    FJ_ORIENTATION_ROTATED180,
    FJ_ORIENTATION_ROTATED270,
    /** Represents a horizontal flip, where pixels in rows are reversed. */
    FJ_ORIENTATION_STANDARD_FLIPPED,
    FJ_ORIENTATION_ROTATED90_FLIPPED,
    FJ_ORIENTATION_ROTATED180_FLIPPED,
    FJ_ORIENTATION_ROTATED270_FLIPPED,

    FJ_ORIENTATION_ENUM32 = INT32_MAX,
};


/** Time in nanoseconds. */
typedef uint64_t fj_time;

/** Dots-per-metres (DPM). */
typedef double fj_density;

/** The length of a metric inch in metres. */
#define FJ_INCH_LENGTH (0.0254)

/**
    Concepts like text/interface scaling factor are mostly derived from the ratio to 96 DPI.
    That is, if the current DPI is 120, the content of the appropriate size is considered to be
    scaled by 120 / 96 = 125% compared to the "unscaled" ("density-unaware") content rendered at
    the standard 96 DPI.
*/
#define FJ_STANDARD_DPI (96.0)


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


static inline fj_time fj_time_from_nanos(uint64_t nanoseconds)
{
    return nanoseconds;
}

static inline uint64_t fj_time_into_nanos(fj_time time)
{
    return time;
}

static inline fj_time fj_time_from_micros(uint64_t microseconds)
{
    return microseconds / UINT64_C(1000);
}

static inline uint64_t fj_time_into_micros(fj_time time)
{
    return time / UINT64_C(1000);
}

static inline fj_time fj_time_from_millis(uint64_t milliseconds)
{
    return milliseconds * UINT64_C(1000000);
}

static inline uint64_t fj_time_into_millis(fj_time time)
{
    return time / UINT64_C(1000000);
}

static inline fj_time fj_time_from_seconds(uint64_t seconds)
{
    return seconds * UINT64_C(1000000000);
}

static inline uint64_t fj_time_into_seconds(fj_time time)
{
    return time / UINT64_C(1000000000);
}


static inline double fj_density_into_dpm(fj_density density)
{
    return density;
}

static inline fj_density fj_density_from_dpm(double dpm)
{
    return dpm;
}

static inline double fj_density_into_dpi(fj_density density)
{
    return density * FJ_INCH_LENGTH;
}

static inline fj_density fj_density_from_dpi(double dpi)
{
    return dpi / FJ_INCH_LENGTH;
}

static inline double fj_density_into_standard_scaling(fj_density density)
{
    return density / FJ_STANDARD_DPI;
}

static inline fj_density fj_density_from_standard_scaling(double scaling_factor)
{
    return scaling_factor * FJ_STANDARD_DPI;
}


/** Always returns a valid printable string, even for invalid status IDs. */
FJ_PUBLIC
char const *fj_error_into_string(enum fj_error e);


#endif
