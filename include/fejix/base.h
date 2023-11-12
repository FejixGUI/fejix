#ifndef FEJIX_BASE_H_
#define FEJIX_BASE_H_


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


/** Annotates a pointer that can be NULL. */
#define FJ_NULLABLE

/** Annotates a pointer to an output variable.
    The value behind the pointer will only be written and never read.
    Therefore, the output variable does not have to be initialised. */
#define FJ_OUT

/** Annotates a pointer that repressents an array. */
#define FJ_ARRAY

/** A value of `fj_err_t` that means that there are no errors. */
#define FJ_OK (NULL)

/** Does the type conversion required to use the string literal as a UTF-8
    string. */
#define FJ_UTF8(STRING_LITERAL) ((fj_string_t)(STRING_LITERAL))

/** Integer numeric type of a fixed size. This is a replacement of `_Bool`,
    which has no specificly defined size.

    === USAGE ===

    Use the standard `true` or `false` for this.

    === CAUTION ===

    This is a numeric type. Therefore, `(fj_bool_t) 0.5 == false` while
    `(bool) 0.5 == true`. */
typedef uint8_t fj_bool_t;

/* UTF-8 null-terminated string. */
typedef uint8_t const * FJ_ARRAY fj_string_t;

/* Mutable UTF-8 null-terminated string. */
typedef uint8_t * FJ_ARRAY fj_string_mut_t;

/* Error message string. */
typedef fj_string_t fj_err_t;


#endif
