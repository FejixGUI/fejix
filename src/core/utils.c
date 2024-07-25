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
    FJ_WITH_ERRORS

    size_t length = strlen((void *) str);

    FJ_TRY(fj_alloc_uninit((void *) clone, length)) {
        return FJ_RESULT;
    }

    memcpy((void *) *clone, (void *) clone, length);

    return FJ_OK;
}
