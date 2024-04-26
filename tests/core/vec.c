#include <fejix/vec.h>
#include <fejix/error.h>
#include <fejix/utils.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


int main(void)
{
    FJ_INIT_TRY

    // TODO better tests

    int _1 = 1;
    int _2 = 2;
    int _3 = 3;

    struct fj_vec v;

    fj_vec_init(&v, sizeof(int));

    FJ_TRY fj_vec_push_item(&v, &_1);
    assert(FJ_RESULT == FJ_OK);

    FJ_TRY fj_vec_push_item(&v, &_2);
    assert(FJ_RESULT == FJ_OK);

    FJ_TRY fj_vec_insert_items(&v, &_3, 0, 1);
    assert(FJ_RESULT == FJ_OK);

    assert(v.length == 3);

    {
        int * v_items = (int *) v.items;
        assert(v_items[0] == 3 && v_items[1] == 1 && v_items[2] == 2);
    }

    FJ_TRY fj_vec_remove_items(&v, 1, 1);
    assert(FJ_RESULT == FJ_OK);

    assert(v.length == 2);

    {
        int * v_items = (int *) v.items;
        assert(v_items[0] == 3 && v_items[1] == 2);
    }

    fj_vec_deinit(&v);

    return 0;
}
