#include "fejix/core/error.h"
#include <fejix/core/utils.h>
#include <fejix/core/malloc.h>

#include <string.h>


fj_bool32_t fj_streq(uint8_t const */*[]*/ a, uint8_t const */*[]*/ b)
{
    return strcmp((char const *)(a), (char const *)(b)) == 0;
}


fj_err_t fj_strdup(uint8_t const */*[]*/ str, uint8_t const */*[]? out*/ * clone)
{
    FJ_INIT_TRY

    size_t length = strlen((void *) str);

    fj_try fj_alloc_uninit((void *) clone, length);
    fj_else {
        return fj_result;
    }

    memcpy((void *) *clone, (void *) clone, length);

    return FJ_OK;
}
