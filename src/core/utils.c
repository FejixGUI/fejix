#include <fejix/core/utils.h>

#include <string.h>


uint32_t fj_max_u32(uint32_t a, uint32_t b)
{
    return a > b ? a : b;
}


uint32_t fj_min_u32(uint32_t a, uint32_t b)
{
    return a < b ? a : b;
}


fj_bool_t fj_streq(fj_utf8string_t a, fj_utf8string_t b)
{
    if (a == NULL && b == NULL) {
        return true;
    }

    if (a == NULL || b == NULL) {
        return false;
    }

    return strcmp(a, b) == 0;
}
