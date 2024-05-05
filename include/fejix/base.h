#ifndef FEJIX_BASE_H_
#define FEJIX_BASE_H_


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


/** Annotates an output function argument. The argument should be write-only.
    This is an opposite of `const`, which annotates read-only values. */
#define fjOUT

/** Annotates a nullable pointer. */
#define fjOPTION

/** Annotates an array pointer. */
#define fjARRAY

/** Annotates a nullable array pointer. */
#define fjARRAY_OPTION

/** Annotates a struct that is a "base class" and is inheritable.
    The inheritance happens by putting the base struct as the first field of
    "derived classes" so that they can be safely dereferenced as the base
    struct. */
#define fjINHERITABLE

/** Annotates the first field of a struct that inherits a base struct. */
#define fjINHERIT

/** Converts (char const *fjARRAY) to (uint8_t const *fjARRAY) */
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

typedef uint64_t fj_nanoseconds_t;

struct fj_ratio {
    int32_t numerator;
    uint32_t denominator;
};

struct fj_position2d {
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


#endif
