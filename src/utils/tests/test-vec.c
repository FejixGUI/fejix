#include <fejix/utils/vector.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


FJ_VECTOR_OF(myvec, int)


int main(void)
{
    // TODO: better tests

    struct myvec v = { 0 };

    int _111 = 111, _222 = 222, _333 = 333;

    assert(myvec_push(&v, &_111) == FJ_STATUS_OK);

    assert(myvec_push(&v, &_222) == FJ_STATUS_OK);

    assert(myvec_insert(&v, 0, &_333) == FJ_STATUS_OK);

    assert(v.length == 3);

    assert(v.items[0] == 333 && v.items[1] == 111 && v.items[2] == 222);

    assert(myvec_remove(&v, 1) == FJ_STATUS_OK);

    assert(v.length == 2);

    assert(v.items[0] == 333 && v.items[1] == 222);

    myvec_free(&v);

    puts("OK");

    return 0;
}
