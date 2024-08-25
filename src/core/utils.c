#include <fejix/core/utils.h>
#include <fejix/core/alloc.h>

#include <string.h>


fj_bool32_t fj_streq(char const */*[]*/ a, char const */*[]*/ b)
{
    return strcmp(a, b) == 0;
}


fj_err_t fj_strdup(char const */*[]*/ str, char const */*[]? out*/ * clone)
{
    FJ_INIT_TRY

    size_t length = strlen(str);

    FJ_TRY(FJ_ARRALLOC_UNINIT(clone, length)) {
        return FJ_RESULT;
    }

    memcpy((void *) *clone, (void *) str, length);

    return FJ_OK;
}


uint32_t fj_u32_next_power_of_two(uint32_t x)
{
    if (x >= UINT32_C(1)<<31) {
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
    if (x >= ~(SIZE_MAX>>1)) {
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


fj_bool32_t fj_dummy_true(void)
{
    return true;
}


fj_bool32_t fj_dummy_false(void)
{
    return false;
}


fj_err_t fj_dummy_ok(void)
{
    return FJ_OK;
}


void fj_dummy_void(void)
{
    return;
}
