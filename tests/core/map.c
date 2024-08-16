#include <fejix/core/map.h>
#include <fejix/core/utils.h>

#include <assert.h>
#include <stdio.h>


int main(void)
{
    FJ_WITH_ERRORS

    struct fj_map m;

    fj_map_init(&m, FJ_TYPE_U32, FJ_TYPE_U32);

    FJ_TRY(fj_map_set(&m, FJ_U32(1), FJ_U32(111))) { assert(false); }

    FJ_TRY(fj_map_set(&m, FJ_U32(2), FJ_U32(222))) { assert(false); }

    FJ_TRY(fj_map_set(&m, FJ_U32(3), FJ_U32(333))) { assert(false); }

    int * a;
    int * b;
    int * c;

    a = fj_map_get(&m, FJ_U32(1));
    b = fj_map_get(&m, FJ_U32(2));
    c = fj_map_get(&m, FJ_U32(3));

    assert(*a == 111);
    assert(*b == 222);
    assert(*c == 333);

    FJ_TRY(fj_map_remove(&m, FJ_U32(2))) { assert(false); }

    a = fj_map_get(&m, FJ_U32(1));
    b = fj_map_get(&m, FJ_U32(2));
    c = fj_map_get(&m, FJ_U32(3));

    assert(*a == 111);
    assert(b == NULL);
    assert(*c == 333);

    FJ_TRY(fj_map_remove(&m, FJ_U32(2))) { assert(false); }

    a = fj_map_get(&m, FJ_U32(1));
    b = fj_map_get(&m, FJ_U32(2));
    c = fj_map_get(&m, FJ_U32(3));

    assert(*a == 111);
    assert(b == NULL);
    assert(*c == 333);

    FJ_TRY(fj_map_remove(&m, FJ_U32(3))) { assert(false); }

    a = fj_map_get(&m, FJ_U32(1));
    b = fj_map_get(&m, FJ_U32(2));
    c = fj_map_get(&m, FJ_U32(3));

    assert(*a == 111);
    assert(b == NULL);
    assert(c == NULL);

    fj_map_deinit(&m);

    puts("OK");

    return 0;
}
