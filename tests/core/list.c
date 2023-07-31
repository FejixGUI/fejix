#include <fejix/fejix.h>
#include <fejix/core/list.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


int main() {

    struct fj_list * list = fj_list_new();

    assert(fj_list_include(list, 123) == fj_ok);
    assert(list->length == 1);
    assert(fj_list_find(list, 123) == 0);
    assert(fj_list_include(list, 123) == fj_ok);
    assert(list->length == 1);
    assert(fj_list_find(list, 123) == 0);

    assert(fj_list_include(list, 321) == fj_ok);
    assert(list->length == 2);
    assert(fj_list_find(list, 321) == 1);
    assert(fj_list_include(list, 321) == fj_ok);
    assert(list->length == 2);
    assert(fj_list_find(list, 321) == 1);

    assert(fj_list_find(list, 123) == 0);
    assert(fj_list_exclude(list, 123) == fj_ok);
    assert(list->length == 1);
    assert(fj_list_find(list, 123) == list->length);
    assert(fj_list_find(list, 321) == 0);

    assert(fj_list_exclude(list, 321) == fj_ok);
    assert(list->length == 0);
    assert(fj_list_find(list, 321) == list->length);


    for (int i=0; i<10; i++) {
        assert(fj_list_insert(list, 0, 9 - i) == fj_ok);
    }

    assert(list->length == 10);

    for (uint32_t i=0; i<list->length; i++) {
        assert(list->elements[i] == i);
    }

    assert(fj_list_pop(list) == fj_ok);
    assert(list->length == 9);
    assert(list->elements[8] == 8);
    assert(fj_list_remove(list, 4) == fj_ok);
    assert(list->length == 8);
    assert(list->elements[7] == 8);
    assert(list->elements[4] == 5);
    assert(list->elements[3] == 3);


    fj_list_del(list);

    return 0;
}
