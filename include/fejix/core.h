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


#ifdef FJ_BUILDING_DOCS

#    define FJ_PUBLIC extern

#else

#    ifdef __cplusplus
#        define FJ_PUBLIC_LINKAGE extern "C"
#    else
#        define FJ_PUBLIC_LINKAGE extern
#    endif

#    if defined(_WIN32)
#        if defined(FJ_BUILDING_PRIVATE_CODE)
#            define FJ_PUBLIC_VISIBILITY __declspec(dllexport)
#        else
#            define FJ_PUBLIC_VISIBILITY __declspec(dllimport)
#        endif
#    elif defined(__GNUC__) && __GNUC__ >= 4
#        define FJ_PUBLIC_VISIBILITY __attribute__((visibility("default")))
#    else
#        define FJ_PUBLIC_VISIBILITY
#    endif

#    define FJ_PUBLIC FJ_PUBLIC_LINKAGE FJ_PUBLIC_VISIBILITY

#endif

#ifdef FJ_BUILDING_PRIVATE_CODE

#    define FJ_OPAQUE_STRUCT_WITH_USERDATA(TYPE) \
        struct TYPE {                            \
            void *userdata;                      \
        };

#else

#    define FJ_OPAQUE_STRUCT_WITH_USERDATA(TYPE) struct TYPE;

#endif


#ifndef FJ_METHOD
#    define FJ_METHOD(NAME, RETURN_TYPE, ...) FJ_PUBLIC RETURN_TYPE (*NAME)(__VA_ARGS__);
#endif

#ifndef FJ_METHOD_NONNULL
#    define FJ_METHOD_NONNULL(NAME, RETURN_TYPE, ...) FJ_METHOD(NAME, RETURN_TYPE, __VA_ARGS__)
#endif

#ifndef FJ_METHOD_LIST_BEGIN
#    define FJ_METHOD_LIST_BEGIN(MODULE_NAME)
#endif

#ifndef FJ_METHOD_LIST_END
#    define FJ_METHOD_LIST_END()
#endif

#ifndef FJ_METHOD_LIST_ITEM
#    define FJ_METHOD_LIST_ITEM(METHOD_NAME)
#endif

#define FJ_TRY(EXPR)                                                                          \
    for (fj_err fj_result = (EXPR), _fj_try_guard = 1; _fj_try_guard == 1; _fj_try_guard = 0) \
        if (fj_result != FJ_OK)


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
