#include <fejix/map.h>
#include <fejix/utils.h>

#include <assert.h>


int main(void)
{
    FJ_INIT_ERRORS

    struct fj_map m;

    char const * a = "a";
    char const * b = "b";
    char const * c = "c";

    fj_map_init(&m);

    FJ_TRY fj_map_set(&m, 1, (void *) a);
    assert(FJ_LAST_ERROR == FJ_OK);

    FJ_TRY fj_map_set(&m, 2, (void *) b);
    assert(FJ_LAST_ERROR == FJ_OK);

    FJ_TRY fj_map_set(&m, 3, (void *) c);
    assert(FJ_LAST_ERROR == FJ_OK);

    assert(fj_map_get(&m, 1) == a);
    assert(fj_map_get(&m, 2) == b);
    assert(fj_map_get(&m, 3) == c);

    FJ_TRY fj_map_set(&m, 2, NULL);
    assert(FJ_LAST_ERROR == FJ_OK);

    assert(fj_map_get(&m, 1) == a);
    assert(fj_map_get(&m, 2) == NULL);
    assert(fj_map_get(&m, 3) == c);

    FJ_TRY fj_map_set(&m, 2, NULL);
    assert(FJ_LAST_ERROR == FJ_OK);

    assert(fj_map_get(&m, 1) == a);
    assert(fj_map_get(&m, 2) == NULL);
    assert(fj_map_get(&m, 3) == c);

    FJ_TRY fj_map_set(&m, 3, NULL);
    assert(FJ_LAST_ERROR == FJ_OK);

    assert(fj_map_get(&m, 1) == a);
    assert(fj_map_get(&m, 2) == NULL);
    assert(fj_map_get(&m, 3) == NULL);

    fj_map_deinit(&m);

    return 0;
}
