#ifndef FEJIX_MAP_H_
#define FEJIX_MAP_H_


#include <fejix/base.h>


struct fj_map_element {
    uintptr_t key;
    void * value;
};

struct fj_map_node {
    struct fj_map_node * next;
    struct fj_map_element element;
};

/* Hash table of `uintptr_t` -> `void *`, whose elements are not NULL. */
struct fj_map {
    struct fj_map_node * * FJ_ARRAY buckets;
    uint32_t bucket_count;
    uint32_t element_count;
};


/* Returns `true` to continue the search or `false` to stop. */
typedef fj_bool_t (*fj_map_foreach_callback_t)(
    struct fj_map_element * element,
    void * data
);


fj_err_t fj_map_init(struct fj_map * map);

void fj_map_deinit(struct fj_map * map);

/* Removes the element if `value` is NULL. */
fj_err_t fj_map_set(struct fj_map * map, uintptr_t key, void * value);

/* Returns NULL if the element was not found. */
void * FJ_NULLABLE fj_map_get(struct fj_map const * map, uintptr_t key);

/* Modifying the map while iterating over it is undefined behavior. */
void fj_map_foreach(
    struct fj_map const * map,
    fj_map_foreach_callback_t callback,
    void * callback_data
);


#endif