#ifndef FEJIX_MAP_H_
#define FEJIX_MAP_H_


#include <fejix/core/types.h>


struct fj_map;
struct fj_map_element;
struct fj_map_iter;
struct fj_map_node;


/// Hash table of `fj_id_t` -> `fj_ptr_t`.
typedef struct fj_map fj_map_t;
typedef struct fj_map_element fj_map_element_t;
typedef struct fj_map_iter fj_map_iter_t;
typedef struct fj_map_node fj_map_node_t;


struct fj_map_element {
    fj_ptr_t value;
    fj_id_t key;
};

struct fj_map_node {
    fj_map_node_t * next;
    fj_map_element_t element;
};

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


fj_map_t * fj_map_new(void);

void fj_map_del(fj_map_t * map);

fj_result_t fj_map_insert(fj_map_t * map, fj_id_t key, fj_ptr_t value);

fj_result_t fj_map_remove(fj_map_t * map, fj_id_t key);

fj_map_element_t * fj_map_find(fj_map_t * map, fj_id_t key);

/// Initialises the iterator, but does not guarantee that it points to an
/// existing element.
/// To point it to the first element, use `fj_map_iter_next`
void fj_map_iter_init(fj_map_t * map, fj_map_iter_t * iter);

/// Returns the current element or NULL if the iterator is exhausted.
/// Modifying the map while iterating over its elements is undefined behavior.
fj_map_element_t * fj_map_iter_next(fj_map_iter_t * iter);


#endif