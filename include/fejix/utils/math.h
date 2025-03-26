#ifndef FEJIX_UTILS_MATH_H_INCLUDED
#define FEJIX_UTILS_MATH_H_INCLUDED


#include <fejix/core.h>


/** Returns a power of two that is higher than x.
    If x >= 2^31, returns UINT32_MAX */
FJ_PUBLIC
uint32_t fj_u32_higher_pow2(uint32_t x);

/** Returns a power of two that is lower than x.
    If x = 0, returns 1. */
FJ_PUBLIC
uint32_t fj_u32_lower_pow2(uint32_t x);

/** If x is greater than or equal to the highest power of two representable by size_t, returns
    SIZE_MAX. */
FJ_PUBLIC
size_t fj_size_higher_pow2(size_t x);

/** If x = 0, returns 1. */
FJ_PUBLIC
size_t fj_size_lower_pow2(size_t x);

/** Returns a multiple of the kernel higher than x. */
FJ_PUBLIC
size_t fj_size_higher_multiple(uint64_t x, size_t kernel);

FJ_INLINE
uint32_t fj_u32_max(uint32_t a, uint32_t b)
{
    return a > b ? a : b;
}

FJ_INLINE
uint32_t fj_u32_min(uint32_t a, uint32_t b)
{
    return a < b ? a : b;
}


#endif
