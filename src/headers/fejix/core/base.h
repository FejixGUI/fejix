#ifndef FEJIX_CORE_BASE_INCLUDED
#define FEJIX_CORE_BASE_INCLUDED


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
#    define FJ_PUBLIC_C_LINKAGE extern "C"
#else
#    define FJ_PUBLIC_C_LINKAGE
#endif

#if defined(_WIN32) && defined(FJ_OPT_PRIVATE)
#    define FJ_PUBLIC_VISIBILITY __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#    define FJ_PUBLIC_VISIBILITY __attribute__((visibility("default")))
#else
#    define FJ_PUBLIC_VISIBILITY
#endif

/**
    Attribute that marks public library functions.
    This typically means exporting function symbols when building a shared library.
    This also adds ``extern "C"`` for C++.
*/
#define FJ_PUBLIC FJ_PUBLIC_C_LINKAGE FJ_PUBLIC_VISIBILITY


#define FJ_VERSION(MAJOR, MINOR, PATCH)                                         \
    ((fj_version_t) ((((MAJOR) & 0xfffff) << 20) | (((MINOR) & 0xfffff) << 10)) \
     | ((PATCH) & 0xfffff))

#define FJ_VERSION_MAJOR(VERSION) ((VERSION) >> 20 & 0xfffff)
#define FJ_VERSION_MINOR(VERSION) ((VERSION) >> 10 & 0xfffff)
#define FJ_VERSION_PATCH(VERSION) ((VERSION) & 0xfffff)
#define FJ_VERSION_NEXT(VERSION) (FJ_VERSION(FJ_VERSION_MAJOR(VERSION) + 1, 0, 0))
#define FJ_VERSION_COMPATIBLE(VERSION, REQUIRED_VERSION) \
    ((VERSION) >= REQUIRED_VERSION && (VERSION) < FJ_VERSION_NEXT(REQUIRED_VERSION))

typedef uint32_t fj_version_t;


/** Error code. */
typedef uint32_t fj_err_t;

/** Error codes. */
enum fj_err {
    /** */
    FJ_OK,
    /** */
    FJ_ERR_UNSUPPORTED,
    /** */
    FJ_ERR_INTERNAL_FAILURE,
    /** */
    FJ_ERR_CANNOT_ALLOCATE,
    /** */
    FJ_ERR_INVALID_ALLOCATION,
    /** */
    FJ_ERR_CANNOT_OPEN_FILE,
    /** */
    FJ_ERR_VECTOR_EMPTY,
    /** */
    FJ_ERR_INVALID_INDEX,
    /** */
    FJ_ERR_CONNECTION_FAILED,
    /** */
    FJ_ERR_INPUT_FAILED,
    /** */
    FJ_ERR_OUTPUT_FAILED,
    /** */
    FJ_ERR_WAITING_FAILED,
    /** */
    FJ_ERR_REQUEST_REJECTED,
    /** */
    FJ_ERR_INVALID_ENCODING,
    /** */
    FJ_ERR_CANNOT_CREATE_TEMPFILE,
    /** */
    FJ_ERR_CANNOT_ALLOCATE_SHARED,

    /** */
    FJ_ERR_PREDEFINED_COUNT,

    /** User-defined errors should begin with this number */
    FJ_ERR_USER = 0x1000,
};


/** Always returns a valid printable string, even for invalid or unknown error IDs. */
FJ_PUBLIC
char const *fj_err_get_description(fj_err_t error);


#endif
