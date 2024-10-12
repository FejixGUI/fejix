#include <fejix/core/map.h>
#include <fejix/core/utils.h>

#include <assert.h>
#include <stdio.h>


int main(void)
{
    struct fj_map m;

    fj_map_init(&m, FJ_TAG_U32, FJ_TAG_U32);

    FJ_TRY (fj_map_set(&m, FJ_TAG(u32, 1), FJ_TAG(u32, 111))) {
        assert(false);
    }

    FJ_TRY (fj_map_set(&m, FJ_TAG(u32, 2), FJ_TAG(u32, 222))) {
        assert(false);
    }

    FJ_TRY (fj_map_set(&m, FJ_TAG(u32, 3), FJ_TAG(u32, 333))) {
        assert(false);
    }

    assert(fj_map_get(&m, FJ_TAG(u32, 1))->u32 == 111);
    assert(fj_map_get(&m, FJ_TAG(u32, 2))->u32 == 222);
    assert(fj_map_get(&m, FJ_TAG(u32, 3))->u32 == 333);

    FJ_TRY (fj_map_remove(&m, FJ_TAG(u32, 2))) {
        assert(false);
    }

    assert(fj_map_get(&m, FJ_TAG(u32, 1))->u32 == 111);
    assert(fj_map_get(&m, FJ_TAG(u32, 2)) == NULL);
    assert(fj_map_get(&m, FJ_TAG(u32, 3))->u32 == 333);

    FJ_TRY (fj_map_remove(&m, FJ_TAG(u32, 2))) {
        assert(false);
    }

    assert(fj_map_get(&m, FJ_TAG(u32, 1))->u32 == 111);
    assert(fj_map_get(&m, FJ_TAG(u32, 2)) == NULL);
    assert(fj_map_get(&m, FJ_TAG(u32, 3))->u32 == 333);

    FJ_TRY (fj_map_remove(&m, FJ_TAG(u32, 3))) {
        assert(false);
    }

    assert(fj_map_get(&m, FJ_TAG(u32, 1))->u32 == 111);
    assert(fj_map_get(&m, FJ_TAG(u32, 2)) == NULL);
    assert(fj_map_get(&m, FJ_TAG(u32, 3)) == NULL);

    fj_map_deinit(&m);

    puts("OK");

    return 0;
}
