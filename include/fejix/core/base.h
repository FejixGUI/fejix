#ifndef FEJIX_BASE_H_
#define FEJIX_BASE_H_


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


#define FJ_OK NULL
#define FJ_MALLOC_FAILED "memory allocation failed"


/* This type has a fixed size, unlike `_Bool`.
    However, use the standard `true` or `false` for this. */
typedef uint8_t fj_bool_t;

/* Unique identifier. */
typedef uint32_t fj_id_t;

/* Untyped pointer. */
typedef void * fj_ptr_t;

/* UTF-8 string for most use cases. */
typedef const char * fj_string_t;

/* ASCII string for internal unique string identifiers.

    String identifiers are inspired by Java package names, for example
    `com.example.SomeProject.some_subproject._1.x` is a valid identifier.

    # Format

    Identifiers must contain only `a-z`, `A-Z`, `_`, `0-9`, `.`
    and must be no longer than 254 characters (including the NULL terminator).

    The parts separated by `.` are called identifier segments.
    Identifier segments must not be empty and must not start with `0-9`.
    There must be at least two segments and thus at least one `.` between
    them. */
typedef const char * fj_string_id_t;

/* ASCII string for error messages. */
typedef const char * fj_err_t;


#endif
