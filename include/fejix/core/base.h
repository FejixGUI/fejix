#ifndef FEJIX_CORE_BASE_H_
#define FEJIX_CORE_BASE_H_


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


/** Makes a tag. :see: :c:union:`fj_tag` */
#define FJ_TAG(TYPE, VALUE) ((union fj_tag) { .TYPE = (VALUE), })

/** Semantic version of MAJOR.MINOR.PATCH, from 0.0.0 to 4096.1024.1024 */
#define FJ_VERSION(MAJOR, MINOR, PATCH) ((fj_version_t) ((MAJOR)<<20) | ((MINOR)<<10) | (PATCH))
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
/** Checks if ``baseMaj.baseMin.0 <= version <= (baseMaj+1).0.0`` */
#define FJ_VERSION_COMPATIBLE(VERSION, BASE_VERSION) \
    (FJ_VERSION_SIGNIFICANT(BASE_VERSION) <= (VERSION) \
    && (VERSION) <= FJ_VERSION_NEXT_SIGNIFICANT(BASE_VERSION))

/** */
typedef uint32_t fj_tag_type_t;

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

/** Error code. */
typedef uint32_t fj_err_t;

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
    FJ_ERR_USER = 10000,
};


/** :see: :c:macro:`FJ_VERSION` */
typedef uint32_t fj_version_t;

/** Use standard true/false for this. */
typedef uint32_t fj_bool32_t;

/** Amount of seconds, accepts INFINITY as "forever" and negatives as "invalid". */
typedef double fj_seconds_t;

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


/** */
fj_bool32_t fj_tag_eq(union fj_tag a, union fj_tag b, fj_tag_type_t type);

/**
Do not hash pointers. When hashing pointers, this converts them to uintptr_t.
Using the result is undefined behavior.
*/
uint32_t fj_tag_hash32(union fj_tag x, fj_tag_type_t type);


#endif
