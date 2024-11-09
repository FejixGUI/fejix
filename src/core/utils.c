#include <fejix/core/alloc.h>
#include <fejix/core/utils.h>

#include <string.h>


fj_bool8_t fj_str_eq(char const *a, char const *b)
{
    return strcmp(a, b) == 0;
}


fj_err_t fj_str_clone(char const *str, char const **clone)
{
    size_t length = strlen(str);

    FJ_TRY (FJ_REALLOC_UNINIT(clone, length)) {
        return fj_result;
    }

    memcpy((void *) *clone, (void *) str, length);

    return FJ_OK;
}


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
