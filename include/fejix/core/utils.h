#ifndef FEJIX_UTILS_H_
#define FEJIX_UTILS_H_


#include <fejix/core/base.h>


/// Get length of a static array
#define FJ_ARRLEN(ARRAY) (sizeof(ARRAY) / sizeof(*(ARRAY))) 

#define FJ_INNER_STRINGIFY(ARG) #ARG
#define FJ_STRINGIFY(ARG) FJ_INNER_STRINGIFY(ARG)

/// Formats the given error message.
/// `MESSAGE` must be a string literal.
#define FJ_ERR(MESSAGE) \
    "Error [" __FILE__ "@" FJ_STRINGIFY(__LINE__) "]: " MESSAGE


uint32_t fj_max(uint32_t a, uint32_t b);

uint32_t fj_min(uint32_t a, uint32_t b);


#endif
