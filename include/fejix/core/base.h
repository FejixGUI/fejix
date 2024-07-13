#ifndef FEJIX_CORE_BASE_H_
#define FEJIX_CORE_BASE_H_


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


/** Converts (char const *) to (uint8_t const *) */
#define FJ_UTF8(STRING_LITERAL) ((uint8_t const *)(void *)(STRING_LITERAL))

/** Creates a unique typedef. Such typedefs are used in public API to make
    pointers to internal structures type-safe. */
#define FJ_DEFINE_UNIQUE_TYPE(TYPE) typedef struct TYPE##_type TYPE;

/** An identifier, which may take only enum values. */
typedef uint32_t fj_enum32_t;

/** Use standard `true`/`false` for this. */
typedef uint32_t fj_bool32_t;

/** Error code. */
typedef fj_enum32_t fj_err_t;

typedef double fj_seconds_t;

struct fj_ratio {
    int32_t numerator;
    uint32_t denominator;
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


#endif
