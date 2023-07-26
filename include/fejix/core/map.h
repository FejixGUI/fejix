#ifndef FEJIX_MAP_H_
#define FEJIX_MAP_H_


#include <fejix/core/types.h>


/// Hash table of `fj_id_t` -> `fj_ptr_t`, whose elements are not NULL.
struct fj_map;

struct fj_map_element {
    fj_ptr_t value;
    fj_id_t key;
};


typedef uint32_t fj_map_foreach_result_t;

typedef fj_map_foreach_result_t (*fj_map_foreach_callback_t)(
    struct fj_map_element * element,
    fj_ptr_t data
);


enum fj_map_foreach_result_values {
    FJ_MAP_FOREACH_CONTINUE = 0,
    FJ_MAP_FOREACH_STOP = 1,
};


/// Returns NULL on failure.
/// If this returns NULL, it can be assumed to be an `FJ_MALLOC_FAIL` error.
struct fj_map * fj_map_new(void);

void fj_map_del(struct fj_map * map);

/// Removes the element if `value` is NULL.
fj_err_t fj_map_set(struct fj_map * map, fj_id_t key, fj_ptr_t value);

/// Returns NULL if the element was not found.
fj_ptr_t fj_map_get(struct fj_map * map, fj_id_t key);

/// Modifying the map while iterating over it is undefined behavior.
void fj_map_foreach(
    struct fj_map * map,
    fj_map_foreach_callback_t callback,
    fj_ptr_t data
);


#endif