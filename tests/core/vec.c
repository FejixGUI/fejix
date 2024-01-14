#include <fejix/vec.h>
#include <fejix/utils.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


int main(void) {

    // TODO better tests

    int _1 = 1;
    int _2 = 2;
    int _3 = 3;

    struct fj_vec v;

    fj_vec_init(&v, sizeof(int));

    fj_vec_push_item(&v, &_1);
    fj_vec_push_item(&v, &_2);
    fj_vec_push_item(&v, &_3);

    assert(v.length == 3);

    {
        int * v_items = FJ_VEC_OF(&v, int);
        assert(v_items[0] == 1 && v_items[1] == 2 && v_items[2] == 3);
    }

    fj_vec_deinit(&v);

    return 0;
}
