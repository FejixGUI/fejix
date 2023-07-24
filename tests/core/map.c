#include <fejix/fejix.h>

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


int main() {
    fj_map_t * map = fj_map_new();

    assert(map != NULL);

    assert(fj_map_insert(map, 1, "a") == FJ_OK);
    assert(fj_map_insert(map, 2, "b") == FJ_OK);
    assert(fj_map_insert(map, 3, "c") == FJ_OK);

    assert(fj_map_find(map, 1) != NULL);
    assert(fj_map_find(map, 2) != NULL);
    assert(fj_map_find(map, 3) != NULL);

    assert(fj_map_remove(map, 2) == FJ_OK);
    assert(fj_map_find(map, 1) != NULL);
    assert(fj_map_find(map, 2) == NULL);
    assert(fj_map_find(map, 3) != NULL);

    assert(fj_map_remove(map, 2) != FJ_OK);
    assert(fj_map_find(map, 1) != NULL);
    assert(fj_map_find(map, 2) == NULL);
    assert(fj_map_find(map, 3) != NULL);

    assert(fj_map_remove(map, 1) == FJ_OK);
    assert(fj_map_find(map, 1) == NULL);
    assert(fj_map_find(map, 2) == NULL);
    assert(fj_map_find(map, 3) != NULL);

    fj_string_t s1 = "s1";
    fj_string_t s2 = "s2";
    assert(fj_map_insert(map, 3, s1) == FJ_OK);
    assert(fj_map_find(map, 3)->value == s1);
    assert(fj_map_insert(map, 3, s2) == FJ_OK);
    assert(fj_map_find(map, 3)->value == s2);
    assert(fj_map_remove(map, 3) == FJ_OK);

    for (int i=0; i<10000; i++) {
        assert(fj_map_insert(map, i, "a") == FJ_OK);
    }

    for (int i=0; i<10000; i++) {
        assert(fj_map_find(map, i) != NULL);
    }

    fj_map_del(map);

    return 0;
}