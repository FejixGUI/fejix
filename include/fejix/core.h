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


#define FJ_VERSION(MAJOR, MINOR, PATCH)                                         \
    ((fj_version_t) ((((MAJOR) & 0xfffff) << 20) | (((MINOR) & 0xfffff) << 10)) \
     | ((PATCH) & 0xfffff))

#define FJ_VERSION_MAJOR(VERSION) ((VERSION) >> 20 & 0xfffff)
#define FJ_VERSION_MINOR(VERSION) ((VERSION) >> 10 & 0xfffff)
#define FJ_VERSION_PATCH(VERSION) ((VERSION) & 0xfffff)

/** Gets the next major version. The next version for `1.2.3` is `2.0.0`. */
#define FJ_VERSION_NEXT(VERSION) (FJ_VERSION(FJ_VERSION_MAJOR(VERSION) + 1, 0, 0))

/**
    Checks if the version is compatible with the required version.
    For example, version `1.2.3` is compatible with versions from `1.2.3` to `2.0.0`.
*/
#define FJ_VERSION_COMPATIBLE(VERSION, WITH_VERSION) \
    ((VERSION) >= WITH_VERSION && (VERSION) < FJ_VERSION_NEXT(WITH_VERSION))

#define FJ_TRY(EXPR)                                                                            \
    for (fj_err_t fj_result = (EXPR), _fj_try_guard = 1; _fj_try_guard == 1; _fj_try_guard = 0) \
        if (fj_result != FJ_OK)  // Outside of for loop to avoid dead code if the block returns


/** Version triple. */
typedef uint32_t fj_version_t;

typedef uint64_t fj_microseconds_t;

/** Error code. */
typedef uint32_t fj_err_t;

enum {
    FJ_OK,

    FJ_ERR_OUT_OF_MEMORY,

    /** The requested operation is not implemented by the library. */
    FJ_ERR_UNIMPLEMENTED,

    /** The requested operation or resource are not available on the system. */
    FJ_ERR_UNAVAILABLE,

    /** Access denied to create a file, share memory, connect to a device etc. */
    FJ_ERR_ACCESS_DENIED,

    /** The request was rejected and there is probably nothing you can do about it. */
    FJ_ERR_REQUEST_REJECTED,

    /** Input/output operation failed. */
    FJ_ERR_IO_FAILED,

    /**
        Invalid usage indicates a programming error like zero allocation size, index out of range,
        removing from an empty vector etc.
    */
    FJ_ERR_INVALID_USAGE,

    /** The requested operation cannot be done on the specified object. */
    FJ_ERR_INVALID_OPERATION,

    /** Invalid text encoding. */
    FJ_ERR_INVALID_ENCODING,

    /** Concurrent access to the object is not permitted. */
    FJ_ERR_CONCURRENT_ACCESS,

    FJ_ERR_PREDEFINED_COUNT,

    /** User-defined errors should begin with this number */
    FJ_ERR_USER = 0x100,
};


/** Always returns a valid printable string, even for invalid or unknown error IDs. */
FJ_PUBLIC
char const *fj_err_get_description(fj_err_t error);


#endif
