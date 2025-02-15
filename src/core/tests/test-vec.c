#include <fejix/core/utils.h>
#include <fejix/core/vec.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    // TODO: better tests

    int _1 = 111;
    int _2 = 222;
    int _3 = 333;

    struct fj_vec v;

    fj_vec_init(&v, sizeof(int));

    FJ_TRY (fj_vec_push(&v, &_1)) {
        assert(false);
    }

    FJ_TRY (fj_vec_push(&v, &_2)) {
        assert(false);
    }

    FJ_TRY (fj_vec_insert(&v, 0, &_3)) {
        assert(false);
    }

    assert(v.length == 3);

    {
        int *items = v.items;
        assert(items[0] == _3 && items[1] == _1 && items[2] == _2);
    }

    FJ_TRY (fj_vec_remove(&v, 1, NULL)) {
        assert(false);
    }

    assert(v.length == 2);

    {
        int *items = v.items;
        assert(items[0] == _3 && items[1] == _2);
    }

    fj_vec_deinit(&v);

    puts("OK");

    return 0;
}
