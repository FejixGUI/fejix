#ifndef FEJIX_UTILS_MATH_H_INCLUDED
#define FEJIX_UTILS_MATH_H_INCLUDED


#include <fejix/core.h>


/** If x >= 2^31, then returns UINT32_MAX */
FJ_PUBLIC
uint32_t fj_u32_next_power_of_two(uint32_t x);

/** If x = 0, then returns 1. */
FJ_PUBLIC
uint32_t fj_u32_prev_power_of_two(uint32_t x);

/**
    If x is greater than or equal to the highest power of two representable by size_t, then returns
    SIZE_MAX.
*/
FJ_PUBLIC
size_t fj_size_next_power_of_two(size_t x);

/** If x = 0, then returns 1. */
FJ_PUBLIC
size_t fj_size_prev_power_of_two(size_t x);

/** Returns the previous multiple of the divisor. */
FJ_PUBLIC
uint32_t fj_u32_prev_multiple(uint32_t x, uint32_t divisor);

/** Returns the previous multiple of the divisor. */
FJ_PUBLIC
uint64_t fj_u64_prev_multiple(uint64_t x, uint64_t divisor);

static inline uint32_t fj_u32_max(uint32_t a, uint32_t b)
{
    return a > b ? a : b;
}

static inline uint32_t fj_u32_min(uint32_t a, uint32_t b)
{
    return a < b ? a : b;
}


#endif
