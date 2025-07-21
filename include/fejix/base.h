#ifndef FEJIX_BASE_H_
#define FEJIX_BASE_H_


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


#if defined(__cplusplus)
#    define FJ_PUBLIC_LINKAGE extern "C"
#else
#    define FJ_PUBLIC_LINKAGE extern
#endif

#if defined(FJ_OPT_DLLEXPORT)
#    define FJ_PUBLIC_VISIBILITY __declspec(dllexport)
#elif defined(FJ_OPT_DLLIMPORT)
#    define FJ_PUBLIC_VISIBILITY __declspec(dllimport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#    define FJ_PUBLIC_VISIBILITY __attribute__((visibility("default")))
#else
#    define FJ_PUBLIC_VISIBILITY
#endif

#define FJ_PUBLIC FJ_PUBLIC_LINKAGE FJ_PUBLIC_VISIBILITY


// This defines the default API implementations that serve as fallbacks
// See src/api_fallbacks.c
#if defined(FJ_OPT_DEFINE_API_FALLBACKS)
#    define FJ_API_EX(FUNCTION_NAME, RETURN_TYPE, DEFAULT_RETURN, ...) \
        static RETURN_TYPE _##FUNCTION_NAME(__VA_ARGS__)               \
        {                                                              \
            return DEFAULT_RETURN;                                     \
        }                                                              \
        FJ_PUBLIC RETURN_TYPE (*FUNCTION_NAME)(__VA_ARGS__);           \
        RETURN_TYPE (*FUNCTION_NAME)(__VA_ARGS__) = _##FUNCTION_NAME;
#else
#    define FJ_API_EX(FUNCTION_NAME, RETURN_TYPE, DEFAULT_RETURN, ...) \
        FJ_PUBLIC RETURN_TYPE (*FUNCTION_NAME)(__VA_ARGS__);
#endif

#define FJ_API(FUNCTION_NAME, ...) \
    FJ_API_EX(FUNCTION_NAME, fj_err, FJ_ERR_UNIMPLEMENTED, __VA_ARGS__)


typedef uint32_t fj_version;

#define FJ_VERSION(MAJOR, MINOR, PATCH) \
    ((MAJOR) << 20 | (MINOR) << 10 | (PATCH))

#define FJ_VERSION_MAJOR(VERSION) ((VERSION) >> 20)
#define FJ_VERSION_MINOR(VERSION) (((VERSION) >> 10) & 0x3FF)
#define FJ_VERSION_PATCH(VERSION) ((VERSION) & 0x3FF)

#define FJ_VERSION_NEXT_MAJOR(VERSION) \
    FJ_VERSION(FJ_VERSION_MAJOR(VERSION) + 1, 0, 0)

#define FJ_VERSION_COMPATIBLE(ACTUAL, WANTED) \
    ((ACTUAL) >= (WANTED) && (ACTUAL) < FJ_VERSION_NEXT_MAJOR(WANTED))

#include <fejix/version.h>


#define FJ_USERDATA(OBJECT) (*((void **) (void *) (OBJECT)))


#define FJ_INCH_LENGTH (0.0254)

#define FJ_STANDARD_DPI (96.0)

enum fj_orientation
{
    FJ_ORIENTATION_STANDARD,
    FJ_ORIENTATION_ROTATED90,
    FJ_ORIENTATION_ROTATED180,
    FJ_ORIENTATION_ROTATED270,
    FJ_ORIENTATION_FLIPPED_STANDARD,
    FJ_ORIENTATION_FLIPPED_ROTATED90,
    FJ_ORIENTATION_FLIPPED_ROTATED180,
    FJ_ORIENTATION_FLIPPED_ROTATED270,
    FJ_ORIENTATION_COUNT,
    FJ_ORIENTATION_ENUM_MAX = INT32_MAX,
};


typedef double fj_density;

struct fj_position2d
{
    uint32_t x;
    uint32_t y;
};

struct fj_offset2d
{
    int32_t x;
    int32_t y;
};

struct fj_size2d
{
    uint32_t width;
    uint32_t height;
};

struct fj_rect2d
{
    struct fj_position2d position;
    struct fj_size2d size;
};

struct fj_viewport2d
{
    struct fj_offset2d offset;
    struct fj_size2d size;
};

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

typedef uint64_t fj_time;

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
    return microseconds * UINT64_C(1000);
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

/** Generic error code.

    These error codes only indicate generic broad domains of problems that may
    have happened. To get a detailed error information, set
    fj_error_callback(). */
typedef enum
{
    /** Success */
    FJ_OK,

    /** Out of memory. */
    FJ_ERR_MEMORY,

    /** System call failed.

        Indicates that a system operation like I/O (read/write/poll/etc.) has
        failed. */
    FJ_ERR_SYSTEM,

    /** Function unimplemented.

        Indicates that the operation is not implemented and therefore no work
        has been done. */
    FJ_ERR_UNIMPLEMENTED,

    /** Unsupported feature.

        Indicates that the platform in general or its specific implementation
        does not support the feature. */
    FJ_ERR_UNSUPPORTED,

    /** Invalid operation.

        Indicates a programming error like zero allocation size, list index out
        of range, bad text encoding, removing from an empty list,
        a thread-unsafe operation done from another thread, using a result
        from an uncompleted task, canceling a completed task etc.

        This may often indicate a bug. */
    FJ_ERR_INVALID,

    /** Host unreachable.

        Indicates that the server (e.g. the graphical system environment) does
        not respond. */
    FJ_ERR_UNREACHABLE,

    /** Request rejected.

        Indicates that the server has responded to the request with an error,
        often indicating that the request has been rejected,
        which may happen because of various reasons from a library bug to
        invalid library usage.

        When possible, the library provides an error message in such cases. */
    FJ_ERR_REJECTED,

    FJ_ERR_COUNT,
    FJ_ERR_ENUM_MAX = INT32_MAX,
} fj_err;

FJ_PUBLIC
void (*fj_error_cb)(char const *message);

FJ_PUBLIC
void *(*fj_allocate_cb)(void *pointer, size_t old_size, size_t new_size);


#endif
