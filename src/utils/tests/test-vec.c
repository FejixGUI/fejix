#include <fejix/utils/vector.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    // TODO: better tests

    struct {
        FJ_VECTOR(int)
    } v = { 0 };

    assert(FJ_VECTOR_EXPAND(&v) == FJ_OK);
    v.items[0] = 111;

    assert(FJ_VECTOR_EXPAND(&v) == FJ_OK);
    v.items[1] = 222;

    assert(FJ_VECTOR_EXPAND_AT(&v, 0) == FJ_OK);
    v.items[0] = 333;

    assert(v.length == 3);

    assert(v.items[0] == 333 && v.items[1] == 111 && v.items[2] == 222);

    assert(FJ_VECTOR_SHRINK_AT(&v, 1) == FJ_OK);

    assert(v.length == 2);

    assert(v.items[0] == 333 && v.items[1] == 222);

    FJ_VECTOR_FREE(&v);

    puts("OK");

    return 0;
}
