#include <src/shared/utils/list.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


FJ_LIST(mylist, int)


int main(void)
{
    // TODO: better tests

    struct mylist l = { 0 };

    int _111 = 111, _222 = 222, _333 = 333;

    assert(mylist_append(&l, &_111) == FJ_OK);

    assert(mylist_append(&l, &_222) == FJ_OK);

    assert(mylist_insert(&l, 0, &_333) == FJ_OK);

    assert(l.length == 3);

    assert(l.items[0] == 333 && l.items[1] == 111 && l.items[2] == 222);

    assert(mylist_remove(&l, 1) == FJ_OK);

    assert(l.length == 2);

    assert(l.items[0] == 333 && l.items[1] == 222);

    mylist_clear(&l);

    puts("OK");

    return 0;
}
