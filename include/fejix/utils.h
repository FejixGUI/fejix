#ifndef FEJIX_UTILS_H_
#define FEJIX_UTILS_H_


#include <fejix/error.h>

#include <string.h>


#define FJ_STRINGIFY(X) FJ_STRINGIFY_IMPL(X)
#define FJ_STRINGIFY_IMPL(X) #X

/** `FJ_FILENAME` is a short relative path to the current file.
    Ideally, our build system defines `FJ_FILENAME` for each source file.
    If this header gets compiled without the build system, this macro
    falls back to `__FILE__`. */
#ifndef FJ_FILENAME
#   define FJ_FILENAME __FILE__
#endif

#define FJ_LINENAME FJ_STRINGIFY(__LINE__)

/** Get length of a fixed-length array. */
#define FJ_ARRAY_LEN(ARRAY) (sizeof(ARRAY) / sizeof((ARRAY)[0]))

/** Double-evaluates the arguments. */
#define FJ_MIN(A, B) (((A) < (B)) ? (A) : (B))

/** Double-evaluates the arguments. */
#define FJ_MAX(A, B) (((A) > (B)) ? (A) : (B))

/** Double-evaluates the arguments. */
#define FJ_CLAMP(X, MIN, MAX) FJ_MAX(FJ_MIN((X), (MAX)), (MIN))

#define FJ_STRING_EQ(A, B) (strcmp((char const *)(A), (char const *)(B)) == 0)


#endif
