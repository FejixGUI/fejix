/**
    .. _headerdoc:

    Defines basic types and utilities.

    API base
    ============

    ..  automacro:: FJ_PUBLIC

    Versions
    ===================

    ..  autotype:: fj_version
    ..  automacro:: FJ_VERSION
    ..  automacro:: FJ_VERSION_MAJOR
    ..  automacro:: FJ_VERSION_MINOR
    ..  automacro:: FJ_VERSION_PATCH
    ..  automacro:: FJ_VERSION_NEXT_MAJOR
    ..  automacro:: FJ_VERSION_COMPATIBLE

    ..  macro:: FJ_HEADER_VERSION

        This is the version of the header file itself, which may differ from a
        version actually implemented by a platform.

        The header version is used to check for API entries that the code can
        use, for example:

        ..  code-block:: c

            #if FJ_VERSION_COMPATIBLE(FJ_HEADER_VERSION, FJ_VERSION(1, 2, 3))
                // use some features from version 1.2.3
            #else
                // use some fallback code
            #endif

        To get the actually implemented version (which may differ per each
        platform and also depends on the actual dynamic library you are linking
        to), use :member:`fj_platform.version`.

    Geometry
    ===============

    ..  automacro:: FJ_INCH_LENGTH
    ..  automacro:: FJ_STANDARD_DPI
    ..  autotype:: fj_density
    ..  autoenum:: fj_orientation
        :members:
    ..  autostruct:: fj_position2d
        :members:
    ..  autostruct:: fj_offset2d
        :members:
    ..  autostruct:: fj_size2d
        :members:
    ..  autostruct:: fj_rect2d
        :members:
    ..  autostruct:: fj_viewport2d
        :members:

    Conversion functions
    ----------------------
    ..  autofunction:: fj_density_into_dpm
    ..  autofunction:: fj_density_from_dpm
    ..  autofunction:: fj_density_into_dpi
    ..  autofunction:: fj_density_from_dpi
    ..  autofunction:: fj_density_into_standard_scaling
    ..  autofunction:: fj_density_from_standard_scaling

    Time
    =============

    ..  autotype:: fj_time

    Conversion functions
    ----------------------
    ..  autofunction:: fj_time_from_nanos
    ..  autofunction:: fj_time_into_nanos
    ..  autofunction:: fj_time_from_micros
    ..  autofunction:: fj_time_into_micros
    ..  autofunction:: fj_time_from_millis
    ..  autofunction:: fj_time_into_millis
    ..  autofunction:: fj_time_from_seconds
    ..  autofunction:: fj_time_into_seconds

    Error handling
    ===================

    ..  autoenum:: fj_err
        :members:
    ..  autovar:: fj_error_callback

    Memory allocation
    ===================

    ..  autovar:: fj_allocate_callback
*///

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

/**
    Defines the appropriate external linkage.
    This is used for all public symbols exported by the library.

    This expands to ``extern`` (``extern "C"`` for C++) and adds special
    attributes for shared libraries if needed.

    ..  warning::
        If you are using the library as shared (``.dll``) on Microsoft Windows,
        you need to define ``FJ_OPT_DLLIMPORT`` before including any library
        headers so that the library gets linked correctly.
        If you are using the library's CMake target, it will automatically do
        that for you.
*/
#define FJ_PUBLIC FJ_PUBLIC_LINKAGE FJ_PUBLIC_VISIBILITY


/**
    A version number in the format of MAJOR.MINOR.PATCH, where each component
    is a 10-bit unsigned integer.
    The maximum value for each component is :math:`2^{10}-1 = 1023`,
    so the maximum version is ``1023.1023.1023``.
    This type is easily comparable: a smaller version number
    corresponds to an older version.
*/
typedef uint32_t fj_version;

/** Constructs a version. */
#define FJ_VERSION(MAJOR, MINOR, PATCH) \
    ((MAJOR) << 20 | (MINOR) << 10 | (PATCH))

/** Gets the major component. */
#define FJ_VERSION_MAJOR(VERSION) ((VERSION) >> 20)
/** Gets the minor component. */
#define FJ_VERSION_MINOR(VERSION) (((VERSION) >> 10) & 0x3FF)
/** Gets the patch component. */
#define FJ_VERSION_PATCH(VERSION) ((VERSION) & 0x3FF)

/** Constructs the next major version, e.g. 1.2.3 -> 2.0.0. */
#define FJ_VERSION_NEXT_MAJOR(VERSION) \
    FJ_VERSION(FJ_VERSION_MAJOR(VERSION) + 1, 0, 0)

/**
    This checks if the ``ACTUAL`` version is compatible with the ``WANTED``
    version using semantic versioning.
*/
#define FJ_VERSION_COMPATIBLE(ACTUAL, WANTED) \
    ((ACTUAL) >= (WANTED) && (ACTUAL) < FJ_VERSION_NEXT_MAJOR(WANTED))

#include <fejix/version.h>


/**
    The length of a metric inch in metres: 0.0254m.

    This is used to convert between dots-per-metre (DPM) and dots-per-inch (DPI)
    and also for standard scaling factor calculations.
*/
#define FJ_INCH_LENGTH (0.0254)

/**
    The (legacy) standard of 96.0 pixels per inch for pixel density.

    Concepts like text/interface scaling factor are mostly derived from the
    ratio to 96 DPI. That is, if the current DPI is 120, the content of the
    appropriate size is considered to be scaled by
    :math:`120 \div 96 = 125\%`
    compared to the "unscaled" ("density-unaware") content rendered at the
    standard 96 DPI.
*/
#define FJ_STANDARD_DPI (96.0)

/** A rectangular orientation, with flips and 90-degree rotations. */
enum fj_orientation
{
    /**
        The standard orientation implies that:

        -   (row 0, pixel 0) is at **top left**
        -   (row 0, pixel 1) is on the **right**
        -   (row 1, pixel 0) is at the **bottom**
    */
    FJ_ORIENTATION_STANDARD,
    /** Rotated 90 degrees clockwise. */
    FJ_ORIENTATION_ROTATED90,
    /** Rotated 180 degrees clockwise. */
    FJ_ORIENTATION_ROTATED180,
    /** Rotated 270 degrees clockwise. */
    FJ_ORIENTATION_ROTATED270,
    /** Horizontally flipped (pixels in a row reversed). */
    FJ_ORIENTATION_FLIPPED_STANDARD,
    /**
        First horizontally flipped (pixels in a row reversed),
        then rotated 90 degrees clockwise.
    */
    FJ_ORIENTATION_FLIPPED_ROTATED90,
    /**
        First horizontally flipped (pixels in a row reversed),
        then rotated 180 degrees clockwise.
    */
    FJ_ORIENTATION_FLIPPED_ROTATED180,
    /**
        First horizontally flipped (pixels in a row reversed),
        then rotated 270 degrees clockwise.
    */
    FJ_ORIENTATION_FLIPPED_ROTATED270,
    FJ_ORIENTATION_COUNT,
    FJ_ORIENTATION_ENUM_MAX = INT32_MAX,
};


/**
    A :expr:`double` representing dots per metre (DPM).

    Convertible to dots per inch (DPI, see :macro:`FJ_INCH_LENGTH`)
    and standard scaling factor (see :macro:`FJ_STANDARD_DPI`).
*/
typedef double fj_density;

/** Represents a position in 2D space. */
struct fj_position2d
{
    /** */
    uint32_t x;
    /** */
    uint32_t y;
};

/** */
struct fj_offset2d
{
    /** */
    int32_t x;
    /** */
    int32_t y;
};

/** */
struct fj_size2d
{
    /** */
    uint32_t width;
    /** */
    uint32_t height;
};

/** Represents a real rectangular part of some visible area. */
struct fj_rect2d
{
    /** */
    struct fj_position2d position;
    /** */
    struct fj_size2d size;
};

/** Represents a rectangle in coordinates relative to some visible area. */
struct fj_viewport2d
{
    /** */
    struct fj_offset2d offset;
    /** */
    struct fj_size2d size;
};

/** */
static inline double fj_density_into_dpm(fj_density density)
{
    return density;
}

/** */
static inline fj_density fj_density_from_dpm(double dpm)
{
    return dpm;
}

/** */
static inline double fj_density_into_dpi(fj_density density)
{
    return density * FJ_INCH_LENGTH;
}

/** */
static inline fj_density fj_density_from_dpi(double dpi)
{
    return dpi / FJ_INCH_LENGTH;
}

/** */
static inline double fj_density_into_standard_scaling(fj_density density)
{
    return density / FJ_STANDARD_DPI;
}

/** */
static inline fj_density fj_density_from_standard_scaling(double scaling_factor)
{
    return scaling_factor * FJ_STANDARD_DPI;
}

/**
    Time interval with nanosecond resolution in range from 1 nanosecond to 584
    years.

    This is used for time intervals, not absolute time, so it does not have a
    specific epoch. It is convertible to/from nanoseconds, microseconds,
    milliseconds and seconds.
*/
typedef uint64_t fj_time;

/** */
static inline fj_time fj_time_from_nanos(uint64_t nanoseconds)
{
    return nanoseconds;
}

/** */
static inline uint64_t fj_time_into_nanos(fj_time time)
{
    return time;
}

/** */
static inline fj_time fj_time_from_micros(uint64_t microseconds)
{
    return microseconds * UINT64_C(1000);
}

/** */
static inline uint64_t fj_time_into_micros(fj_time time)
{
    return time / UINT64_C(1000);
}

/** */
static inline fj_time fj_time_from_millis(uint64_t milliseconds)
{
    return milliseconds * UINT64_C(1000000);
}

/** */
static inline uint64_t fj_time_into_millis(fj_time time)
{
    return time / UINT64_C(1000000);
}

/** */
static inline fj_time fj_time_from_seconds(uint64_t seconds)
{
    return seconds * UINT64_C(1000000000);
}

/** */
static inline uint64_t fj_time_into_seconds(fj_time time)
{
    return time / UINT64_C(1000000000);
}

/**
    A generic error code.

    These error codes only indicate generic broad domains of problems that may
    have happened. To get a detailed error information, set
    :func:`fj_error_cb`.
*/
typedef enum
{
    /** Success */
    FJ_OK,

    /** Out of memory */
    FJ_ERR_INSUFFICIENT_MEMORY,

    /**
        Operation or service unsupported.

        Indicates that the platform in general or its specific implementation
        does not support the feature.
    */
    FJ_ERR_UNSUPPORTED,

    /**
        Invalid usage (indicates programming errors): zero allocation size,
        list index out of range, bad text encoding, removing from an empty list,
        a thread-unsafe operation done from another thread, etc.

        This may often indicate a bug.
    */
    FJ_ERR_INVALID_USAGE,

    /**
        An operation resulted in a fault.

        Indicates that the library encountered an error while
        processing a request. The error likely comes from the server and
        could not be prevented internally.
    */
    FJ_ERR_FAULT,

    /**
        The system is broken.

        Indicates a critical failure: a system service became unreachable,
        or something crashed.
    */
    FJ_ERR_BROKEN_SYSTEM,

    FJ_ERR_COUNT,
    FJ_ERR_ENUM_MAX = INT32_MAX,
} fj_err;

/**
    Called every time the library sets an error message.

    This is initialized to a default callback that does nothing for release
    builds and prints errors to stderr for debug builds.

    This function *must not* store the message pointer as the string may be
    freed immediately afterwards. The string must be copied in order to be
    stored.

    This should be thread-safe as some operations may be performed from other
    threads like :func:`fj_app_ping`.
*/
FJ_PUBLIC
void (*fj_error_callback)(char const *message);

/**
    Called every time the library needs to manage memory allocation.

    This can be set to a custom function that will be used for all memory
    allocations done by the library.

    The default implementation uses standard C library functions
    ``malloc()``, ``realloc()``, and ``free()``.

    The callback does not need to be thread-safe as it is only called from
    the main thread.

    :param old_size:
        This can be 0 indicating that the memory must be newly allocated.

    :param new_size:
        This is never equal to ``old_size``.
        This can be 0 indicating that the memory must be freed.

    :returns: NULL on allocation failure or when freeing.
*/
FJ_PUBLIC
void *(*fj_allocate_callback)(void *pointer, size_t old_size, size_t new_size);


#endif
