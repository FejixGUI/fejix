/**
    \file
*/

#ifndef FEJIX_CORE_H_INCLUDED
#define FEJIX_CORE_H_INCLUDED


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


#define FJ_VERSION_MAJOR (0)
#define FJ_VERSION_MINOR (1)
#define FJ_VERSION_PATCH (0)


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


/** Status code. */
enum fj_status {
    /** Success */
    FJ_STATUS_OK,

    /** The requested operation is not implemented and therefore no work has been done. */
    FJ_STATUS_UNIMPLEMENTED,

    /** Out of memory */
    FJ_STATUS_OUT_OF_MEMORY,

    /** Input/output operation failed. */
    FJ_STATUS_IO_FAILED,

    /** The requested operation or resource are not available on the system. */
    FJ_STATUS_UNAVAILABLE,

    /** Access denied to create a file, share memory, connect to a device etc. */
    FJ_STATUS_ACCESS_DENIED,

    /** Concurrent access to the object is not permitted. */
    FJ_STATUS_CONCURRENT_ACCESS,

    /** Invalid usage indicates a programming error like zero allocation size, index out of range,
        removing from an empty vector etc. */
    FJ_STATUS_INVALID_USAGE,

    /** The requested operation cannot be done on the specified object. */
    FJ_STATUS_INVALID_OPERATION,

    /** Invalid text encoding. */
    FJ_STATUS_INVALID_ENCODING,

    /** The operation has failed, a generic error returned when concrete reasons are unknown. */
    FJ_STATUS_OPERATION_FAILED,

    FJ_STATUS_MAX,

    FJ_STATUS_ENUM32 = INT32_MAX,
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


/** Always returns a valid printable string, even for invalid status IDs. */
FJ_PUBLIC
char const *fj_status_get_description(enum fj_status s);

FJ_PUBLIC
void fj_get_backends(char const *const **out_backends, uint32_t *out_backends_length);

/**
    \returns NULL if there are no backends built in, otherwise always returns a valid backend.
*/
FJ_PUBLIC
char const *fj_get_default_backend(void);

/**
    \param backend_name If NULL, the default backend is initialised.
    \returns Error if the specified backend is not built into the library.
*/
FJ_PUBLIC
enum fj_status fj_init(char const *backend_name);


#endif
