#ifndef FEJIX_UTILS_MATH_H_
#define FEJIX_UTILS_MATH_H_


#include <fejix/base.h>


static inline uint32_t fj_u32_max(uint32_t a, uint32_t b)
{
    return a > b ? a : b;
}

static inline uint32_t fj_u32_min(uint32_t a, uint32_t b)
{
    return a < b ? a : b;
}

static inline size_t fj_size_max(size_t a, size_t b)
{
    return a > b ? a : b;
}

static inline size_t fj_size_min(size_t a, size_t b)
{
    return a < b ? a : b;
}


/** Returns a power of two that is higher than x.
    If x >= 2^31, returns UINT32_MAX */
uint32_t fj_u32_higher_pow2(uint32_t x);

/** Returns a power of two that is lower than x.
    If x = 0, returns 1. */
uint32_t fj_u32_lower_pow2(uint32_t x);

/** If x is greater than or equal to the highest power of two representable by size_t, returns
    SIZE_MAX. */
size_t fj_size_higher_pow2(size_t x);

/** If x = 0, returns 1. */
size_t fj_size_lower_pow2(size_t x);

/** Returns a multiple of the base that is also higher than x. */
static inline size_t fj_size_higher_multiple(uint64_t x, size_t base)
{
    return x + (base - x % base);
}


#endif
