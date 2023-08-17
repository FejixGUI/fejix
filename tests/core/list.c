#include <fejix/prelude.h>
#include <fejix/core/list.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


typedef uint32_t u32;


int main() {

    struct fj_list * list = fj_list_new(sizeof(uint32_t));

    // TODO Better tests...

    u32 a = 123;
    u32 b = 321;

    assert(fj_list_push(list, &a) == FJ_OK);
    assert(list->length == 1);
    assert(fj_list_find(list, &a) == 0);

    assert(fj_list_push(list, &a) == FJ_OK);
    assert(list->length == 2);
    assert(fj_list_find(list, &a) == 0);

    assert(*(u32*)fj_list_get(list, 0) == a);
    assert(*(u32*)fj_list_get(list, 1) == a);

    assert(fj_list_insert(list, 1, &b) == FJ_OK);
    assert(list->length == 3);
    assert(fj_list_find(list, &b) == 1);

    assert(fj_list_push(list, &b) == FJ_OK);
    assert(list->length == 4);
    assert(fj_list_find(list, &b) == 1);

    assert(*(u32*)fj_list_get(list, 1) == b);
    assert(*(u32*)fj_list_get(list, 3) == b);

    assert(fj_list_exclude(list, &a) == FJ_OK);
    assert(list->length == 3);
    assert(fj_list_find(list, &a) == 1);

    assert(fj_list_exclude(list, &b) == FJ_OK);
    assert(list->length == 2);
    assert(fj_list_find(list, &b) == 1);

    assert(fj_list_remove(list, 1) == FJ_OK);
    assert(list->length == 1);
    assert(*(u32*)fj_list_get(list, 0) == a);

    assert(fj_list_remove(list, 0) == FJ_OK);
    assert(list->length == 0);

    for (u32 i=0; i<10; i++) {
        u32 item = 9 - i;
        assert(fj_list_insert(list, 0, &item) == FJ_OK);
    }

    assert(list->length == 10);

    for (u32 i=0; i<list->length; i++) {
        assert(*(u32*) fj_list_get(list, i) == i);
    }

    assert(fj_list_pop(list) == FJ_OK);
    assert(list->length == 9);
    assert(*(u32*) fj_list_get(list, 8) == 8);
    assert(fj_list_remove(list, 4) == FJ_OK);
    assert(list->length == 8);
    assert(*(u32*)fj_list_get(list, 7) == 8);
    assert(*(u32*)fj_list_get(list, 4) == 5);
    assert(*(u32*)fj_list_get(list, 3) == 3);


    fj_list_del(list);

    return 0;
}
