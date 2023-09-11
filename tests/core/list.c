#include <fejix/core/list.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


int main() {

    struct fj_list * list = fj_list_new(sizeof(uint32_t));

    // TODO Better tests...

    uint32_t a = 123;

    assert(fj_list_push(list, &a) == FJ_OK);
    assert(fj_list_push(list, &a) == FJ_OK);
    assert(fj_list_push(list, &a) == FJ_OK);
    assert(*(uint32_t*)fj_list_get(list, 2) == a);

    fj_list_del(list);

    return 0;
}
