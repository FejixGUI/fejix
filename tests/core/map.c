#include <fejix/fejix.h>

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


int main() {
    fj_map_t * map = fj_map_new();

    assert(map != NULL);

    fj_result_t result = FJ_OK;

    result = fj_map_insert(map, 1, "a");
    assert(result == FJ_OK);
    result = fj_map_insert(map, 2, "b");
    assert(result == FJ_OK);
    result = fj_map_insert(map, 3, "c");
    assert(result == FJ_OK);

    assert(fj_map_find(map, 1) != NULL);
    assert(fj_map_find(map, 2) != NULL);
    assert(fj_map_find(map, 3) != NULL);

    fj_map_iter_t iter;
    fj_map_iter_init(map, &iter);
    while (fj_map_iter_next(&iter) != FJ_MAP_ITER_FINISHED) {
        fj_map_element_t * elem = fj_map_iter_get_element(&iter);
        printf("%d %s\n", elem->key, (char *) elem->value);
    }

    result = fj_map_remove(map, 2);
    assert(result == FJ_OK);
    assert(fj_map_find(map, 1) != NULL);
    assert(fj_map_find(map, 2) == NULL);
    assert(fj_map_find(map, 3) != NULL);

    result = fj_map_remove(map, 2);
    assert(result != FJ_OK);
    assert(fj_map_find(map, 1) != NULL);
    assert(fj_map_find(map, 2) == NULL);
    assert(fj_map_find(map, 3) != NULL);

    result = fj_map_remove(map, 1);
    assert(result == FJ_OK);
    assert(fj_map_find(map, 1) == NULL);
    assert(fj_map_find(map, 2) == NULL);
    assert(fj_map_find(map, 3) != NULL);

    for (int i=0; i<10000; i++) {
        result = fj_map_insert(map, i, "a");
        assert(result == FJ_OK);
    }

    for (int i=0; i<10000; i++) {
        assert(fj_map_find(map, i) != NULL);
    }

    fj_map_del(map);

    return 0;
}