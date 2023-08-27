#ifndef FEJIX_BASE_H_
#define FEJIX_BASE_H_


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


#define FJ_OK NULL


/* This type has a fixed size, unlike `_Bool`.
    However, use the standard `true` or `false` for this. */
typedef uint8_t fj_bool_t;

/* Untyped pointer. */
typedef void * fj_ptr_t;

// TODO Delete this eventually. The concept of IDs was deprecated with the new API
typedef uint32_t fj_id_t;

/* UTF-8 string for most use cases. */
typedef const char * fj_utf8string_t;

/* ASCII string for rare use cases. */
typedef const char * fj_asciistring_t;

/* ASCII unique string identifiers.

    String identifiers are inspired by Java package names, for example
    `com.example.some_project.some_subproject._1.x` is a valid identifier.

    # Format

    Identifiers must contain only `a-z`, `_`, `0-9`, `.`
    and must be no longer than 127 characters (including the NULL terminator).

    The parts separated by `.` are called identifier segments.
    Identifier segments must not be empty and must not start with `0-9`.
    There must be at least two segments and thus at least one `.` between
    them. */
typedef fj_asciistring_t fj_idstring_t;

/* ASCII string for error messages. */
typedef fj_asciistring_t fj_err_t;


#endif
