#ifndef FEJIX_BASE_H_
#define FEJIX_BASE_H_


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


#ifdef UINT64_MAX
#   define FJ_HAS_UINT64
#endif


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
#define FJ_UTF8(STRING_LITERAL) ((fj_string_t)(void *) (STRING_LITERAL))

#define FJ_FIRST_MESSAGE_OF(SOCKET_ID) ((SOCKET_ID)<<16)

/** Makes a version number from two unsigned integers (max is UINT16_MAX). */
#define FJ_VERSION(MAJOR, MINOR) \
    ((((fj_version_t)(MAJOR)) << 16) | ((fj_version_t)(MINOR)))

/** Gets the major component of the version. */
#define FJ_VERSION_MAJOR(VERSION) ((VERSION) >> 16)

/** Gets the minor component of the version. */
#define FJ_VERSION_MINOR(VERSION) ((VERSION) & 0x0000FFFF)


/** Identifier of bus, socket, message or capability. */
typedef uint32_t fj_id_t;


/** Represents a MAJOR.MINOR tuple.
    The higher half contains the major component.
    The lower half contains the minor component.
    Therefore, it is safe to compare versions as normal numbers. */
typedef uint32_t fj_version_t;

/** Integer numeric type of a fixed size. This is a replacement of `_Bool`,
    which has no specificly defined size.

    === USAGE ===

    Use the standard `true` or `false` for this.

    === CAUTION ===

    This is a numeric type. Therefore, `(fj_bool_t) 0.5 == false` while
    `(bool) 0.5 == true`. Never convert arbitrary objects to booleans. */
typedef uint8_t fj_bool_t;

/* UTF-8 null-terminated string. */
typedef uint8_t const * FJ_ARRAY fj_string_t;

/* Mutable UTF-8 null-terminated string. */
typedef uint8_t * FJ_ARRAY fj_string_mut_t;

/* Error message string. */
typedef fj_string_t fj_err_t;


enum fj_id_limits {
    FJ_ID_BUS_MIN       = 0x00000001,
    FJ_ID_BUS_MAX       = 0x000000FF,
    FJ_ID_SOCKET_MIN    = 0x00000100,
    FJ_ID_SOCKET_MAX    = 0x00007FFF,
    FJ_ID_MESSAGE_MIN   = 0x00100000,
    FJ_ID_MESSAGE_MAX   = 0x7FFFFFFF,
};


#endif
