#ifndef FEJIX_MAP_H_
#define FEJIX_MAP_H_


#include <fejix/core/types.h>


struct fj_map_element;
struct fj_map_node;
struct fj_map;
struct fj_map_iter;

typedef struct fj_map_element fj_map_element_t;
typedef struct fj_map_node fj_map_node_t;
typedef struct fj_map fj_map_t;
typedef struct fj_map_iter fj_map_iter_t;

typedef uint32_t fj_map_iter_status_t;

enum fj_map_iter_status_values {
    FJ_MAP_ITER_CONTINUED = 0,
    FJ_MAP_ITER_FINISHED = 1,
};


struct fj_map_element {
    fj_ptr_t value;
    fj_id_t key;
};

struct fj_map_node {
    fj_map_node_t * next;
    fj_map_element_t element;
};

/// @struct fj_map
///
/// Hash table of `fj_id_t` -> `fj_ptr_t`.
struct fj_map {
    fj_map_node_t ** buckets;
    uint32_t buckets_count;
    uint32_t elements_count;
};

struct fj_map_iter {
    fj_map_t * map;
    fj_map_node_t * current_node;
    uint32_t current_bucket_index;
};


fj_map_t * fj_map_new();

void fj_map_del(fj_map_t * map);

fj_result_t fj_map_insert(fj_map_t * map, fj_id_t key, fj_ptr_t value);

fj_result_t fj_map_remove(fj_map_t * map, fj_id_t key);

fj_map_element_t * fj_map_find(fj_map_t * map, fj_id_t key);

/// Initialises the iterator, but does not guarantee that it points to an
/// existing element.
/// To point it to the first element, use `fj_map_iter_next`
void fj_map_iter_init(fj_map_t * map, fj_map_iter_t * iter);

fj_map_iter_status_t fj_map_iter_next(fj_map_iter_t * iter);

fj_map_element_t * fj_map_iter_get_element(fj_map_iter_t * iter);


#endif