#include <fejix/map.h>
#include <fejix/error.h>
#include <fejix/utils.h>

#include <assert.h>


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

    FJ_TRY fj_map_set(&m, FJ_U32(1), FJ_PTR(a));
    assert(FJ_RESULT == FJ_OK);

    FJ_TRY fj_map_set(&m, FJ_U32(2), FJ_PTR(b));
    assert(FJ_RESULT == FJ_OK);

    FJ_TRY fj_map_set(&m, FJ_U32(3), FJ_PTR(c));
    assert(FJ_RESULT == FJ_OK);

    ap = fj_map_get(&m, FJ_U32(1));
    bp = fj_map_get(&m, FJ_U32(2));
    cp = fj_map_get(&m, FJ_U32(3));

    assert(*ap == a);
    assert(*bp == b);
    assert(*cp == c);

    FJ_TRY fj_map_remove(&m, FJ_U32(2));
    assert(FJ_RESULT == FJ_OK);

    ap = fj_map_get(&m, FJ_U32(1));
    bp = fj_map_get(&m, FJ_U32(2));
    cp = fj_map_get(&m, FJ_U32(3));

    assert(*ap == a);
    assert(bp == NULL);
    assert(*cp == c);

    FJ_TRY fj_map_remove(&m, FJ_U32(2));
    assert(FJ_RESULT == FJ_OK);

    ap = fj_map_get(&m, FJ_U32(1));
    bp = fj_map_get(&m, FJ_U32(2));
    cp = fj_map_get(&m, FJ_U32(3));

    assert(*ap == a);
    assert(bp == NULL);
    assert(*cp == c);

    FJ_TRY fj_map_remove(&m, FJ_U32(3));
    assert(FJ_RESULT == FJ_OK);

    ap = fj_map_get(&m, FJ_U32(1));
    bp = fj_map_get(&m, FJ_U32(2));
    cp = fj_map_get(&m, FJ_U32(3));

    assert(*ap == a);
    assert(bp == NULL);
    assert(cp == NULL);

    fj_map_deinit(&m);

    return 0;
}
