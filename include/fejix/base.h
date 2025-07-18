/** \file
    Defines base definitions and utilities. */

#ifndef FEJIX_BASE_H_
#define FEJIX_BASE_H_


#include <fejix/version.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


/// \addtogroup base_api
/// \{

#if defined(FJ_OPT_DOCS)

/** Defines the appropriate external linkage.
    This is used for all public symbols exported by the library.

    This expands to `extern` (`extern "C"` for C++) and adds special attributes
    for shared libraries if needed.

    \warning
    If you are using the library as shared (`.dll`) on Microsoft Windows, you
    need to define `FJ_OPT_DLLIMPORT` before including any library
    headers so that the library gets linked correctly.
    If you are using the library's CMake target, it will automatically do that
    for you. */
#    define FJ_PUBLIC extern /* ... attributes ... */

#else

#    if defined(__cplusplus)
#        define FJ_PUBLIC_LINKAGE extern "C"
#    else
#        define FJ_PUBLIC_LINKAGE extern
#    endif

#    if defined(FJ_OPT_DLLEXPORT)
#        define FJ_PUBLIC_VISIBILITY __declspec(dllexport)
#    elif defined(FJ_OPT_DLLIMPORT)
#        define FJ_PUBLIC_VISIBILITY __declspec(dllimport)
#    elif defined(__GNUC__) && __GNUC__ >= 4
#        define FJ_PUBLIC_VISIBILITY __attribute__((visibility("default")))
#    else
#        define FJ_PUBLIC_VISIBILITY
#    endif

#    define FJ_PUBLIC FJ_PUBLIC_LINKAGE FJ_PUBLIC_VISIBILITY

#endif


#if defined(FJ_OPT_PRIVATE_CODE)
#    define FJ_PUBLICLY(X)
#else
#    define FJ_PUBLICLY(X) X
#endif

/// \}


/// \addtogroup base_version
/// \{

struct fj_version
{
    uint16_t major;
    uint16_t minor;
    uint16_t patch;
};

/// \}


/// \addtogroup base_geometry
/// \{

/** The length of a metric inch in metres. */
#define FJ_INCH_LENGTH (0.0254)

/** Concepts like text/interface scaling factor are mostly derived from the
    ratio to 96 DPI. That is, if the current DPI is 120, the content of the
    appropriate size is considered to be scaled by \f$120 \div 96 = 125\%\f$
    compared to the "unscaled" ("density-unaware") content rendered at the
    standard 96 DPI. */
#define FJ_STANDARD_DPI (96.0)


/** A rectangular orientation, with flips and 90-degree rotations. */
enum fj_orientation
{
    /** The standard orientation implies that:
        - (row 0, pixel 0) is at TOP LEFT
        - (row 0, pixel 1) points RIGHT
        - (row 1, pixel 0) points DOWN */
    FJ_ORIENTATION_STANDARD,

    /** Rotated 90 degrees clockwise. */
    FJ_ORIENTATION_ROTATED90,
    /** Rotated 180 degrees clockwise. */
    FJ_ORIENTATION_ROTATED180,
    /** Rotated 270 degrees clockwise. */
    FJ_ORIENTATION_ROTATED270,
    /** Horizontally flipped (pixels in a row reversed). */
    FJ_ORIENTATION_FLIPPED_STANDARD,
    /** First horizontally flipped (pixels in a row reversed),
        then rotated 90 degrees clockwise. */
    FJ_ORIENTATION_FLIPPED_ROTATED90,
    /** First horizontally flipped (pixels in a row reversed),
        then rotated 180 degrees clockwise. */
    FJ_ORIENTATION_FLIPPED_ROTATED180,
    /** First horizontally flipped  (pixels in a row reversed),
        then rotated 270 degrees clockwise. */
    FJ_ORIENTATION_FLIPPED_ROTATED270,

    FJ_ORIENTATION_COUNT,
    FJ_ORIENTATION_ENUM_MAX = INT32_MAX,
};


/** Dots-per-metre (DPM).

    Convertible to dots-per-inch (DPI, see #FJ_INCH_LENGTH)
    and standard scaling factor (see #FJ_STANDARD_DPI). */
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

/** Represents a real rectangular part of some visible area. */
struct fj_rect2d
{
    struct fj_position2d position;
    struct fj_size2d size;
};

/** Represents a rectangle in coordinates relative to some visible area. */
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

/// \}


/// \addtogroup base_time
/// \{

/** Time interval with nanosecond resolution in range from 1 nanosecond to 584
    years. */
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

/// \}

/// \addtogroup base_error_handling
/// \{

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

/** Called every time the library sets an error message.

    This is initialized to a default callback that does nothing for release
    builds and prints errors to stderr for debug builds.

    This function *must not* store the message pointer as the string may be
    freed immediately afterwards. The string must be copied in order to be
    stored.

    This should be thread-safe as some operations may be performed from other
    threads like #FJ_APP_PING. */
FJ_PUBLIC
void (*fj_error_callback)(char const *message);

/// \}

/// \addtogroup base_memory_management
/// \{

/** Called every time the library needs to manage memory allocation.

    \param old_size
        This can be 0 indicating that the memory must be newly allocated.

    \param new_size
        This is never equal to \p old_size.
        This can be 0 indicating that the memory must be freed.

    \returns NULL on allocation failure or when freeing.

    \noop TODO Allocation thread-safety

    This does not have to be thread-safe. */
FJ_PUBLIC
void *(*fj_allocation_callback)(
    void *pointer, size_t old_size, size_t new_size);

/// \}


/// \addtogroup base_object_oriented
/// \{

/** A generic dispatcher type.

    You should not call dispatchers with this type directly, but convert it
    to the specific dispatcher type you need. The reason is that calling
    functions through pointers of the wrong type is UB in C and is compiler
    and platform-dependent. However, on some platforms, this may work.

    \noop TODO On what platforms does this work? */
typedef fj_err (*fj_generic_dispatcher)(
    void *object, uint32_t message, void *message_data);


enum fj_message_limits
{
    /** This is for messages used internally in the library, such messages
        should be dispatched with the default dispatcher and not handled in any
        other way. */
    FJ_PRIVATE_MESSAGE_MIN = 5000,

    /** You can implement your own custom messages that start from
        #FJ_USER_MESSAGE_MIN (if you really need to). */
    FJ_USER_MESSAGE_MIN = 10000,
};

enum fj_type
{
    FJ_TYPE_APP,
    FJ_TYPE_WINDOW,
    FJ_TYPE_WINDOW_SERVICE,

    FJ_TYPE_COUNT,
    FJ_TYPE_ENUM_MAX = INT32_MAX,
};

/// \}


#endif
