#include <fejix/fejix.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


int main() {

    fj_result_t res;

    fj_idlist_t * list = fj_idlist_new();

    for (int i=0; i<10000; i++) {
        res = fj_idlist_insert(list, 0, i);
        assert(res == FJ_OK);
    }

    for (int i=0; i<list->length; i++) {
        assert(list->elements[i] == 10000-i-1);
    }

    fj_idlist_del(list);

    return 0;
}