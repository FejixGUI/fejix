#ifndef FEJIX_UTILS_H_
#define FEJIX_UTILS_H_


#include <fejix/core/base.h>
#include <fejix/core/malloc.h>


/* Get length of a static array */
#define FJ_ARRLEN(ARRAY) (sizeof(ARRAY) / sizeof(*(ARRAY))) 

#define FJ_STRINGIFY(ARG) FJ_STRINGIFY2(ARG)
#define FJ_STRINGIFY2(ARG) #ARG

/* `FJ_UTIL_FILENAME` is a short relative path to the current file.
    Ideally, CMake defines `FJ_FILENAME` for each Fejix source file.
    If this header gets compiled without CMake, it falls back to
    compiler-specific macros or to `__FILE__` */
#if defined(FJ_FILENAME)
#   define FJ_UTIL_FILENAME FJ_FILENAME
#elif defined(__FILE_NAME__)
#   define FJ_UTIL_FILENAME __FILE_NAME__
#elif defined(__FILE_BASENAME__)
#   define FJ_UTIL_FILENAME __FILE_BASENAME__
#else
#   define FJ_UTIL_FILENAME __FILE__
#endif

#define FJ_UTIL_LINE FJ_STRINGIFY(__LINE__)

/* Formats the given error message. `TEXT` must be a string literal. */
#define FJ_ERR(TEXT) "error [" FJ_UTIL_FILENAME ":" FJ_UTIL_LINE "]: " TEXT

/* Double-evaluates the arguments */
#define FJ_MAX(A, B) (((A) > (B)) ? (A) : (B))

/* Double-evaluates the arguments */
#define FJ_MIN(A, B) (((A) < (B)) ? (A) : (B))


#endif
