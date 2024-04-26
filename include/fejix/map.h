#ifndef FEJIX_MAP_H_
#define FEJIX_MAP_H_


#include <fejix/base.h>


struct fj_map_element {
    uintptr_t key;
    void * value;
};

struct fj_map_node {
    struct fj_map_node *fjOPTION next;
    struct fj_map_element element;
};

/** Hash table of `uintptr_t` -> `void *`, where values are not NULL. */
struct fj_map {
    uint32_t element_count;
    uint32_t bucket_count;
    struct fj_map_node *fjOPTION *fjARRAY_OPTION buckets;
};

struct fj_map_iter {
    struct fj_map const * map;
    uint32_t bucket_index;
    struct fj_map_node *fjOPTION current_node;
};


void fj_map_init(
    struct fj_map * map
);

void fj_map_deinit(
    struct fj_map * map
);

fj_bool32_t fj_map_is_empty(
    struct fj_map const * map
);

fj_bool32_t fj_map_has_allocated(
    struct fj_map const * map
);

/** Removes the element if `value` is NULL. */
fj_err_t fj_map_set(
    struct fj_map * map,
    uintptr_t key,
    void *fjOPTION value
);

/** Returns NULL if the element was not found. */
void *fjOPTION fj_map_get(
    struct fj_map const * map,
    uintptr_t key
);


void fj_map_iter_init(
    struct fj_map_iter fjOUT * iter,
    struct fj_map const * map
);

fj_bool32_t fj_map_iter_finished(
    struct fj_map_iter const * iter
);

/** Returns true if the next element was successfully found. */
fj_bool32_t fj_map_iter_next(
    struct fj_map_iter * iter,
    struct fj_map_element *fjOPTION fjOUT * element
);


#endif
