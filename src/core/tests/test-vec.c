#include <fejix/core/utils.h>
#include <fejix/core/vec.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    // TODO: better tests

    int _1 = 1;
    int _2 = 2;
    int _3 = 3;

    struct fj_vec v;

    fj_vec_init(&v, sizeof(int));

    FJ_TRY (fj_vec_push(&v, &_1)) {
        assert(false);
    }

    FJ_TRY (fj_vec_push(&v, &_2)) {
        assert(false);
    }

    FJ_TRY (fj_vec_insert(&v, &_3, 0, 1)) {
        assert(false);
    }

    assert(v.length == 3);

    {
        int *v_items = (int *) v.items;
        assert(v_items[0] == 3 && v_items[1] == 1 && v_items[2] == 2);
    }

    FJ_TRY (fj_vec_remove(&v, 1, 1)) {
        assert(false);
    }

    assert(v.length == 2);

    {
        int *v_items = (int *) v.items;
        assert(v_items[0] == 3 && v_items[1] == 2);
    }

    fj_vec_deinit(&v);

    puts("OK");

    return 0;
}
