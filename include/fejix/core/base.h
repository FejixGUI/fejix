#ifndef FEJIX_CORE_BASE_H_
#define FEJIX_CORE_BASE_H_


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


#define FJ_DEFINE_OPAQUE_TYPE(TYPE) typedef struct TYPE TYPE;

#define FJ_OBJECT(SOME_PTR) ((struct fj_object const *)(void const *) (SOME_PTR))

/** Semantic version of MAJOR.MINOR.PATCH, min 0.0.0, max 1024.1024.1024 */
#define FJ_VERSION(MAJOR, MINOR, PATCH) ((fj_version_t) ((MAJOR)<<20) | ((MINOR)<<10) | (PATCH))
#define FJ_VERSION_MAJOR(VERSION) (((VERSION) >> 20) & 0x3FF)
#define FJ_VERSION_MINOR(VERSION) (((VERSION) >> 10) & 0x3FF)
#define FJ_VERSION_PATCH(VERSION) ((VERSION) & 0x3FF)
#define FJ_VERSION_COMPATIBLE(VERSION, BASE_VERSION) \
    ((BASE_VERSION)<=(VERSION) && (VERSION)<=FJ_VERSION(FJ_VERSION_MAJOR(BASE_VERSION)+1, 0, 0))


/** Error code. */
typedef uint32_t fj_err_t;

enum fj_err {
    FJ_OK = 0,

    FJ_ERR_UNKNOWN,
    FJ_ERR_UNSUPPORTED,
    FJ_ERR_OUT_OF_MEMORY,
    FJ_ERR_INVALID_ALLOCATION,
    FJ_ERR_IO_ERROR,
    FJ_ERR_MESSAGE_READ_ERROR,
    FJ_ERR_MESSAGE_SEND_ERROR,
    FJ_ERR_SHELL_CONNECTION_ERROR,
    FJ_ERR_INVALID_TEXT_ENCODING,

    FJ_ERR_MAX,
};


typedef uint32_t fj_version_t;

/** Use standard `true`/`false` for this. */
typedef uint32_t fj_bool32_t;

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


#endif
