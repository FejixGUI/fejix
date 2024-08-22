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
