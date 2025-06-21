#include "math.h"


uint32_t fj_u32_higher_pow2(uint32_t x)
{
    if (x >= UINT32_C(1) << 31) {
        return UINT32_MAX;
    }

    uint32_t y = 1;

    while (y <= x) {
        y *= 2;
    }

    return y;
}


uint32_t fj_u32_lower_pow2(uint32_t x)
{
    if (x == 0) {
        return 1;
    }

    uint32_t y = 1;

    while (y < x - y) {
        y *= 2;
    }

    return y;
}


size_t fj_size_higher_pow2(size_t x)
{
    if (x >= ~(SIZE_MAX >> 1)) {
        return SIZE_MAX;
    }

    size_t y = 1;

    while (y <= x) {
        y *= 2;
    }

    return y;
}


size_t fj_size_lower_pow2(size_t x)
{
    if (x == 0) {
        return 1;
    }

    size_t y = 1;

    while (y < x - y) {
        y *= 2;
    }

    return y;
}


size_t fj_size_higher_multiple(uint64_t x, size_t kernel)
{
    return x + (kernel - x % kernel);
}
