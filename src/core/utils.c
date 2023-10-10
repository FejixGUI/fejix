#include <fejix/utils.h>

#include <string.h>


uint32_t fj_u32_max(uint32_t a, uint32_t b)
{
    return a > b ? a : b;
}


uint32_t fj_u32_min(uint32_t a, uint32_t b)
{
    return a < b ? a : b;
}


fj_bool_t fj_str_eq(fj_string_t a, fj_string_t b)
{
    if (a == NULL && b == NULL) {
        return true;
    }

    if (a == NULL || b == NULL) {
        return false;
    }

    return strcmp((char const *) a, (char const *) b) == 0;
}
