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

    If using the library compiled as shared (`.dll`) on Microsoft Windows, you need to define
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

/**
    Forward-declares a struct.

    Even though each backend may define the struct in its own way, the first field is always the
    userdata and is accessible with #FJ_USERDATA.

    This is here primarily because Doxygen does not recognize struct forward declarations, so we
    actually define it a bit differently in Doxygen to make the structs appear in the docs.
*/
#define FJ_OBJECT(TYPE) struct TYPE;

#if !defined(FJ_METHOD) || defined(FJ_COMPILE_OPT_DOCS)
/**
    Defines a function pointer which can be NULL.

    The function pointer may be NULL if unimplemented by the currently selected backend.
*/
#    define FJ_METHOD(NAME, RETURN_TYPE, ...) FJ_PUBLIC RETURN_TYPE (*NAME)(__VA_ARGS__);
#endif

#if !defined(FJ_METHOD_NONNULL) || defined(FJ_COMPILE_OPT_DOCS)
/**
    Defines a function pointer which cannot be NULL.

    Calling the function results in #FJ_ERROR_UNIMPLEMENTED if the method is unimplemented by the
    currently selected backend.
*/
#    define FJ_METHOD_NONNULL(NAME, RETURN_TYPE, ...) FJ_METHOD(NAME, RETURN_TYPE, __VA_ARGS__)
#endif

#if !defined(FJ_METHOD_LIST_BEGIN)
#    define FJ_METHOD_LIST_BEGIN(MODULE_NAME)
#endif

#if !defined(FJ_METHOD_LIST_END)
#    define FJ_METHOD_LIST_END()
#endif

#if !defined(FJ_METHOD_LIST_ITEM)
#    define FJ_METHOD_LIST_ITEM(METHOD_NAME)
#endif

/**
    Retrieves the object's userdata as its first field.
*/
#define FJ_USERDATA(OBJECT) (*(void **) (OBJECT))


/** Error code. */
enum fj_error {
    /** Success */
    FJ_OK = 0,

    /** Out of memory */
    FJ_ERROR_OUT_OF_MEMORY = 1,

    /** The requested operation is not implemented and therefore no work has been done. */
    FJ_ERROR_UNIMPLEMENTED = 2,

    /** The operation has failed, a generic error returned when concrete reasons are unknown. */
    FJ_ERROR_OPERATION_FAILED = 3,

    /** Input/output operation failed. */
    FJ_ERROR_IO_FAILED = 4,

    /** The requested operation or resource are not available on the system. */
    FJ_ERROR_UNAVAILABLE = 5,

    /** Access denied to create a file, share memory, connect to a device etc. */
    FJ_ERROR_ACCESS_DENIED = 6,

    /** Concurrent access to the object is not permitted. */
    FJ_ERROR_CONCURRENT_ACCESS = 7,

    /** Invalid usage indicates a programming error like zero allocation size, index out of range,
        removing from an empty vector etc. */
    FJ_ERROR_INVALID_USAGE = 8,

    /** The requested operation cannot be done on the specified object. */
    FJ_ERROR_INVALID_OPERATION = 9,

    /** Invalid text encoding. */
    FJ_ERROR_INVALID_ENCODING = 10,

    FJ_ERROR_ENUM32 = INT32_MAX,
};


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
char const *fj_error_get_description(enum fj_error error);


#endif
