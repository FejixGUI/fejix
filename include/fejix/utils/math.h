#ifndef FEJIX_UTILS_MATH_H_INCLUDED
#define FEJIX_UTILS_MATH_H_INCLUDED


#include <fejix/core.h>


/** The length of a metric inch in metres. */
#define FJ_INCH_LENGTH (0.0254)

/**
    Concepts like text/interface scaling factor are mostly derived from the ratio to 96 DPI.
    That is, if the current DPI is 120, the content of the appropriate size is considered to be
    scaled by 120 / 96 = 125% compared to the "unscaled" ("density-unaware") content rendered at
    the standard 96 DPI.
*/
#define FJ_STANDARD_DPI (96.0)


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

static inline uint32_t fj_u32_max(uint32_t a, uint32_t b)
{
    return a > b ? a : b;
}

static inline uint32_t fj_u32_min(uint32_t a, uint32_t b)
{
    return a < b ? a : b;
}


static inline fj_time fj_time_from_nanos(uint64_t nanoseconds)
{
    return nanoseconds;
}

static inline uint64_t fj_time_into_nanos(fj_time time)
{
    return time;
}

static inline fj_time fj_time_from_micros(uint64_t microseconds)
{
    return microseconds / UINT64_C(1000);
}

static inline uint64_t fj_time_into_micros(fj_time time)
{
    return time / UINT64_C(1000);
}

static inline fj_time fj_time_from_millis(uint64_t milliseconds)
{
    return milliseconds * UINT64_C(1000000);
}

static inline uint64_t fj_time_into_millis(fj_time time)
{
    return time / UINT64_C(1000000);
}

static inline fj_time fj_time_from_seconds(uint64_t seconds)
{
    return seconds * UINT64_C(1000000000);
}

static inline uint64_t fj_time_into_seconds(fj_time time)
{
    return time / UINT64_C(1000000000);
}


static inline double fj_density_into_dpm(fj_density density)
{
    return density;
}

static inline fj_density fj_density_from_dpm(double dpm)
{
    return dpm;
}

static inline double fj_density_into_dpi(fj_density density)
{
    return density * FJ_INCH_LENGTH;
}

static inline fj_density fj_density_from_dpi(double dpi)
{
    return dpi / FJ_INCH_LENGTH;
}

static inline double fj_density_into_standard_scaling(fj_density density)
{
    return density / FJ_STANDARD_DPI;
}

static inline fj_density fj_density_from_standard_scaling(double scaling_factor)
{
    return scaling_factor * FJ_STANDARD_DPI;
}


#endif
