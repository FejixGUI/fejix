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


/** An identifier, which may take only enum values. */
typedef uint32_t fj_enum32_t;

/** Use standard `true`/`false` for this. */
typedef uint32_t fj_bool32_t;

/** Error code. */
typedef fj_enum32_t fj_err_t;

typedef uint64_t fj_nanoseconds_t;

/** Represents a rational number of p/q. */
struct fj_ratio {
    int32_t p;
    uint32_t q;
};

#endif
