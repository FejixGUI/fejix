#ifndef FEJIX_CORE_BASE_H_
#define FEJIX_CORE_BASE_H_


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


#define FJ_DEFINE_OPAQUE_TYPE(TYPE) typedef struct TYPE TYPE;

#define FJ_AS_UTF8(STRING) ((uint8_t const *)(STRING))
#define FJ_AS_CSTR(STRING) ((char const *)(STRING))

#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L) \
    || (defined(__cplusplus) && __cplusplus >= 201103L)
#   define FJ_UTF8(STRING_LITERAL) (FJ_AS_UTF8(u8##STRING_LITERAL))
#else
#   define FJ_UTF8(STRING_LITERAL) (FJ_AS_UTF8(STRING_LITERAL))
#endif

/** Semantic version of MAJOR.MINOR.PATCH, min 0.0.0, max 1024.1024.1024 */
#define FJ_VERSION(MAJOR, MINOR, PATCH) ((fj_version_t) ((MAJOR)<<20) | ((MINOR)<<10) | (PATCH))
#define FJ_VERSION_MAJOR(VERSION) (((VERSION) >> 20) & 0x3FF)
#define FJ_VERSION_MINOR(VERSION) (((VERSION) >> 10) & 0x3FF)
#define FJ_VERSION_PATCH(VERSION) ((VERSION) & 0x3FF)
#define FJ_VERSION_COMPATIBLE(VERSION, BASE_VERSION) \
    ((BASE_VERSION)<=(VERSION) && (VERSION)<=FJ_VERSION(FJ_VERSION_MAJOR(BASE_VERSION)+1, 0, 0))


/** Use standard `true`/`false` for this. */
typedef uint32_t fj_bool32_t;

/** Error code. */
typedef uint32_t fj_err_t;

typedef double fj_seconds_t;

typedef uint32_t fj_version_t;


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


#endif
