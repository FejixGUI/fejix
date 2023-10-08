#ifndef FEJIX_BASE_H_
#define FEJIX_BASE_H_


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


/* Annotates a pointer that can be NULL */
#define FJ_NULLABLE

/* Annotates a pointer to an output variable */
#define FJ_OUT

/* Annotates a pointer to a variable that can be an output just as an input */
#define FJ_INOUT

/* Annotates a function that must be initialised by the user */
#define FJ_CALLBACK

/* A value of `fj_err_t` that means that there are no errors. */
#define FJ_OK NULL

/* Does the type conversion required to use string literals as `fj_string_t` */
#define FJ_STR(STRING_LITERAL) ((fj_string_t)(STRING_LITERAL))


/* This type has a fixed size, unlike `_Bool`.
    Use the standard `true` or `false` for this.

    **Be careful:** this is an integral numeric type.
    Therefore, `(fj_bool_t) 0.5 == false` while `(bool) 0.5 == true` */
typedef uint8_t fj_bool_t;

/* UTF-8 string. */
typedef const uint8_t * fj_string_t;

/* Mutable UTF-8 string. */
typedef uint8_t * fj_string_mut_t;

/* Error message string. */
typedef fj_string_t fj_err_t;


#endif
