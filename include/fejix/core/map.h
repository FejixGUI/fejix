#ifndef FEJIX_CORE_MAP_H_
#define FEJIX_CORE_MAP_H_


#include <fejix/core/base.h>
#include <fejix/core/any.h>


struct fj_map_element {
    union fj_any key;
    union fj_any value;
};

struct fj_map_node {
    struct fj_map_node */*?*/ next;
    struct fj_map_element element;
};

struct fj_map {
    fj_any_type_t key_type;
    fj_any_type_t value_type;
    uint32_t element_count;
    uint32_t bucket_count;
    struct fj_map_node */*?*/ */*[]?*/ buckets;
};

struct fj_map_iter {
    struct fj_map const * map;
    uint32_t bucket_index;
    struct fj_map_node */*?*/ current_node;
};


void fj_map_init(struct fj_map /*out*/ * map, fj_any_type_t key_type, fj_any_type_t value_type);

void fj_map_deinit(struct fj_map * map);

fj_bool32_t fj_map_is_empty(struct fj_map const * map);

fj_bool32_t fj_map_has_allocated(struct fj_map const * map);

fj_err_t fj_map_set(struct fj_map * map, union fj_any key, union fj_any value);

/** Returns the pointer to value. Returns NULL if the element was not found. */
void */*?*/ fj_map_get(struct fj_map const * map, union fj_any key);

fj_err_t fj_map_remove(struct fj_map * map, union fj_any key);


void fj_map_iter_init(struct fj_map_iter /*out*/ * iter, struct fj_map const * map);

fj_bool32_t fj_map_iter_finished(struct fj_map_iter const * iter);

/** Returns true if the next element was successfully found. */
fj_bool32_t fj_map_iter_next(
    struct fj_map_iter * iter,
    struct fj_map_element */*? out*/ * element
);


#endif
