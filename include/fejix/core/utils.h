#ifndef FEJIX_UTILS_H_
#define FEJIX_UTILS_H_


#include <fejix/core/base.h>


/* Get length of a static array. */
#define FJ_ARRLEN(ARRAY) (sizeof(ARRAY) / sizeof(*(ARRAY))) 

#define FJ_STRINGIFY(ARG) FJ_STRINGIFY_IMPL(ARG)
#define FJ_STRINGIFY_IMPL(ARG) #ARG

/* `FJ_UTIL_FILENAME` is a short relative path to the current file.
    Ideally, CMake defines `FJ_FILENAME` for each Fejix source file.
    If this header gets compiled without CMake, it falls back to `__FILE__`. */
#ifdef FJ_FILENAME
#   define FJ_UTIL_FILENAME FJ_FILENAME
#else
#   define FJ_UTIL_FILENAME __FILE__
#endif

#define FJ_UTIL_LINE FJ_STRINGIFY(__LINE__)

/* Formats the given error message. `TEXT` must be a string literal. */
#define FJ_ERR(TEXT) ("error [" FJ_UTIL_FILENAME ":" FJ_UTIL_LINE "]: " TEXT)

/* Example: `FJ_ERR(FJ_MALLOC_FAILED)` */
#define FJ_MALLOC_FAILED "memory allocation failed"

uint32_t fj_max_u32(uint32_t a, uint32_t b);
uint32_t fj_min_u32(uint32_t a, uint32_t b);

/* Accepts NULL as arguments. `NULL==NULL`, but `NULL!=""` */
fj_bool_t fj_streq(fj_utf8string_t a, fj_utf8string_t b);

#endif
