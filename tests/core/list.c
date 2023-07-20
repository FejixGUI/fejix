#include <fejix/fejix.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


int main() {

    fj_idlist_t * list = fj_idlist_new();

    for (int i=0; i<100; i++) {
        fj_idlist_insert(list, 0, i);
    }

    // for (int i=0; i<10000; i++) {
    //     assert(list->elements[i] == 10000-i-1);
    // }

    for (int i=0; i<100; i++) {
        printf("%d ", list->elements[i]);
    }

    fj_idlist_del(list);

    return 0;
}