#include <fejix/core/map.h>
#include <fejix/core/utils.h>

#include <assert.h>
#include <stdio.h>


int main(void)
{
    struct fj_map m;

    fj_map_init(&m, FJ_TAG_U32, FJ_TAG_U32);

    union fj_tag t1 = { .u32 = 1 }, t2 = { .u32 = 2 }, t3 = { .u32 = 3 }, t111 = { .u32 = 111 },
                 t222 = { .u32 = 222 }, t333 = { .u32 = 333 };

    FJ_TRY (fj_map_set(&m, t1, t111)) {
        assert(false);
    }

    FJ_TRY (fj_map_set(&m, t2, t222)) {
        assert(false);
    }

    FJ_TRY (fj_map_set(&m, t3, t333)) {
        assert(false);
    }

    assert(fj_map_get(&m, t1)->u32 == 111);
    assert(fj_map_get(&m, t2)->u32 == 222);
    assert(fj_map_get(&m, t3)->u32 == 333);

    FJ_TRY (fj_map_remove(&m, t2)) {
        assert(false);
    }

    assert(fj_map_get(&m, t1)->u32 == 111);
    assert(fj_map_get(&m, t2) == NULL);
    assert(fj_map_get(&m, t3)->u32 == 333);

    FJ_TRY (fj_map_remove(&m, t2)) {
        assert(false);
    }

    assert(fj_map_get(&m, t1)->u32 == 111);
    assert(fj_map_get(&m, t2) == NULL);
    assert(fj_map_get(&m, t3)->u32 == 333);

    FJ_TRY (fj_map_remove(&m, t3)) {
        assert(false);
    }

    assert(fj_map_get(&m, t1)->u32 == 111);
    assert(fj_map_get(&m, t2) == NULL);
    assert(fj_map_get(&m, t3) == NULL);

    fj_map_deinit(&m);

    puts("OK");

    return 0;
}
