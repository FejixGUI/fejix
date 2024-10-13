#ifndef FEJIX_CORE_BASE_H_
#define FEJIX_CORE_BASE_H_


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


#ifdef FJ_OPT_INTERNAL
#    define FJ_PUBLICLY(X)
#    define FJ_INTERNALLY(X) X
#else
#    define FJ_PUBLICLY(X) X
#    define FJ_INTERNALLY(X)
#endif

#ifdef __cplusplus
#    define FJ_EXTERN_C_BEGIN extern "C" {
#    define FJ_EXTERN_C_END }
#else
#    define FJ_EXTERN_C_BEGIN
#    define FJ_EXTERN_C_END
#endif


/** Makes a tag. :see: :c:union:`fj_tag` */
#define FJ_TAG(VARIANT, VALUE) ((union fj_tag) { .VARIANT = (VALUE) })

/** Semantic version of MAJOR.MINOR.PATCH, from 0.0.0 to 4096.1024.1024 */
#define FJ_VERSION(MAJOR, MINOR, PATCH) ((fj_version_t) ((MAJOR) << 20) | ((MINOR) << 10) | (PATCH))
/** Gets the major part of the version. */
#define FJ_VERSION_MAJOR(VERSION) (((VERSION) >> 20) & 0xFFF)
/** Gets the minor part of the version. */
#define FJ_VERSION_MINOR(VERSION) (((VERSION) >> 10) & 0x3FF)
/** Gets the patch part of the version. */
#define FJ_VERSION_PATCH(VERSION) ((VERSION) & 0x3FF)
/** Gets the major and minor parts of the version. */
#define FJ_VERSION_SIGNIFICANT(VERSION) (FJ_VERSION_MAJOR(VERSION) | FJ_VERSION_MINOR(VERSION))
/** Makes the next major version. */
#define FJ_VERSION_NEXT_SIGNIFICANT(VERSION) (FJ_VERSION(FJ_VERSION_MAJOR(BASE_VERSION) + 1, 0, 0))
/** Checks if ``base_major.base_minor.0 <= version <= (base_major+1).0.0`` */
#define FJ_VERSION_COMPATIBLE(VERSION, BASE_VERSION)   \
    (FJ_VERSION_SIGNIFICANT(BASE_VERSION) <= (VERSION) \
     && (VERSION) <= FJ_VERSION_NEXT_SIGNIFICANT(BASE_VERSION))

#define FJ_TIMEOUT_FROM_SECS(SECONDS) ((fj_timeout_t) (SECONDS) * UINT64_C(1000000000))
#define FJ_TIMEOUT_FROM_MILLIS(MILLISECONDS) ((fj_timeout_t) (MILLISECONDS) * UINT64_C(1000000))
#define FJ_TIMEOUT_FROM_MICROS(MICROSECONDS) ((fj_timeout_t) (MICROSECONDS) * UINT64_C(1000))
#define FJ_TIMEOUT_FROM_NANOS(NANOSECONDS) ((fj_timeout_t) (NANOSECONDS))

#define FJ_TIMEOUT_FROM_FLOAT_SECS(SECONDS) ((fj_timeout_t) ((SECONDS) * 1e+9))
#define FJ_TIMEOUT_FROM_FLOAT_MILLIS(MILLISECONDS) ((fj_timeout_t) ((MILLISECONDS) * 1e+6))
#define FJ_TIMEOUT_FROM_FLOAT_MICROS(MICROSECONDS) ((fj_timeout_t) ((MICROSECONDS) * 1e+3))
#define FJ_TIMEOUT_FROM_FLOAT_NANOS(NANOSECONDS) ((fj_timeout_t) (NANOSECONDS))

#define FJ_TIMEOUT_SECS(TIMEOUT) ((TIMEOUT) / 1000000000LL)
#define FJ_TIMEOUT_MILLIS(TIMEOUT) ((TIMEOUT) / 1000000LL)
#define FJ_TIMEOUT_MICROS(TIMEOUT) ((TIMEOUT) / 1000LL)
#define FJ_TIMEOUT_NANOS(TIMEOUT) (TIMEOUT)


/** */
enum fj_tag_type {
    /** */
    FJ_TAG_U32,
    /** */
    FJ_TAG_I32,
    /** */
    FJ_TAG_U64,
    /** */
    FJ_TAG_I64,
    /** */
    FJ_TAG_UPTR,
    /** */
    FJ_TAG_IPTR,
    /** */
    FJ_TAG_PTR,
};

/** Error codes. */
enum fj_err {
    FJ_OK,
    FJ_ERR_UNKNOWN,
    FJ_ERR_ALLOCATION_FAILED,
    FJ_ERR_INVALID_ALLOCATION,
    FJ_ERR_CONNECTION_FAILED,
    FJ_ERR_REQUEST_SENDING_FAILED,
    FJ_ERR_REQUEST_FAILED,
    FJ_ERR_EVENT_WAITING_FAILED,
    FJ_ERR_EVENT_READING_FAILED,
    FJ_ERR_INVALID_TEXT_ENCODING,
    FJ_ERR_INTERFACE_INIT_FAILED,
    FJ_ERR_TEMP_FILE_CREATION_FAILED,
    FJ_ERR_SHARED_MEMORY_ALLOCATION_FAILED,

    FJ_ERR_MAX,

    /** User-defined errors should begin with this number */
    FJ_ERR_USER = 0x10000,
};


/** Error code. */
typedef uint32_t fj_err_t;

/** */
typedef uint32_t fj_tag_type_t;

/** :see: :c:macro:`FJ_VERSION` */
typedef uint32_t fj_version_t;

/** Use standard ``true``/``false`` for this. */
typedef uint8_t fj_bool8_t;

/** Represents timeout in nanoseconds for short-term waiting. */
typedef uint64_t fj_timeout_t;


/**
Tag is something that can identify an object, e.g. a pointer to the object, an ID of the object
or any other pointer or numeric value.
*/
union fj_tag {
    /** */
    uint32_t u32;
    /** */
    int32_t i32;
    /** */
    uint64_t u64;
    /** */
    int64_t i64;
    /** */
    uintptr_t uptr;
    /** */
    intptr_t iptr;
    /** */
    void *ptr;
};


struct fj_position2d {
    uint32_t x;
    uint32_t y;
};

struct fj_offset2d {
    int32_t x;
    int32_t y;
};

struct fj_size2d {
    uint32_t width;
    uint32_t height;
};

struct fj_rect2d {
    struct fj_position2d position;
    struct fj_size2d size;
};

struct fj_viewport2d {
    struct fj_offset2d offset;
    struct fj_size2d size;
};


FJ_EXTERN_C_BEGIN


/** */
fj_bool8_t fj_tag_eq(union fj_tag a, union fj_tag b, fj_tag_type_t type);

/**
Do not hash pointers. When hashing pointers, this converts them to uintptr_t.
Using the result is undefined behavior.
*/
uint32_t fj_tag_hash32(union fj_tag x, fj_tag_type_t type);


FJ_EXTERN_C_END

#endif
