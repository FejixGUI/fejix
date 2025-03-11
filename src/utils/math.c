#include <fejix/utils/math.h>


uint32_t fj_u32_next_power_of_two(uint32_t x)
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


uint32_t fj_u32_prev_power_of_two(uint32_t x)
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


size_t fj_size_next_power_of_two(size_t x)
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


size_t fj_size_prev_power_of_two(size_t x)
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


uint32_t fj_u32_prev_multiple(uint32_t x, uint32_t divisor)
{
    return x - x % divisor;
}


uint64_t fj_u64_prev_multiple(uint64_t x, uint64_t divisor)
{
    return x - x % divisor;
}
