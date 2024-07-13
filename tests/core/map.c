#include <fejix/core/map.h>
#include <fejix/core/error.h>
#include <fejix/core/utils.h>

#include <assert.h>
#include <stdio.h>


int main(void)
{
    FJ_INIT_TRY

    struct fj_map m;

    char const * a = "a";
    char const * b = "b";
    char const * c = "c";

    char const * const * ap;
    char const * const * bp;
    char const * const * cp;

    fj_map_init(&m, FJ_TYPE_U32, FJ_TYPE_PTR);

    fj_try fj_map_set(&m, FJ_U32(1), FJ_PTR(a));
    assert(fj_result == FJ_OK);

    fj_try fj_map_set(&m, FJ_U32(2), FJ_PTR(b));
    assert(fj_result == FJ_OK);

    fj_try fj_map_set(&m, FJ_U32(3), FJ_PTR(c));
    assert(fj_result == FJ_OK);

    ap = fj_map_get(&m, FJ_U32(1));
    bp = fj_map_get(&m, FJ_U32(2));
    cp = fj_map_get(&m, FJ_U32(3));

    assert(*ap == a);
    assert(*bp == b);
    assert(*cp == c);

    fj_try fj_map_remove(&m, FJ_U32(2));
    assert(fj_result == FJ_OK);

    ap = fj_map_get(&m, FJ_U32(1));
    bp = fj_map_get(&m, FJ_U32(2));
    cp = fj_map_get(&m, FJ_U32(3));

    assert(*ap == a);
    assert(bp == NULL);
    assert(*cp == c);

    fj_try fj_map_remove(&m, FJ_U32(2));
    assert(fj_result == FJ_OK);

    ap = fj_map_get(&m, FJ_U32(1));
    bp = fj_map_get(&m, FJ_U32(2));
    cp = fj_map_get(&m, FJ_U32(3));

    assert(*ap == a);
    assert(bp == NULL);
    assert(*cp == c);

    fj_try fj_map_remove(&m, FJ_U32(3));
    assert(fj_result == FJ_OK);

    ap = fj_map_get(&m, FJ_U32(1));
    bp = fj_map_get(&m, FJ_U32(2));
    cp = fj_map_get(&m, FJ_U32(3));

    assert(*ap == a);
    assert(bp == NULL);
    assert(cp == NULL);

    fj_map_deinit(&m);

    puts("OK");

    return 0;
}
