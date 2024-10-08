#ifndef FEJIX_CORE_MAP_H_
#define FEJIX_CORE_MAP_H_


#include <fejix/core/base.h>


/** */
struct fj_map_element {
    /** */
    union fj_tag key;
    /** */
    union fj_tag value;
};

struct fj_map_node {
    /** NULL if this is the last node in a sequence. */
    struct fj_map_node *next;
    struct fj_map_element element;
};

/** Map of tag -> tag. */
struct fj_map {
    fj_tag_type_t key_type;
    fj_tag_type_t value_type;

    /** Number of stored elements. */
    uint32_t element_count;

    uint32_t bucket_count;

    /* Nullable array of buckets (NULL if the map has not allocated).
    A bucket is a head node pointer (if it is occupied) or NULL (if it is empty). */
    struct fj_map_node **buckets;
};

/** Iterator over map. */
struct fj_map_iter {
    struct fj_map const *map;
    uint32_t bucket_index;

    /* Can be NULL. */
    struct fj_map_node *current_node;
};


/** Initialises the map, never allocates. */
void fj_map_init(struct fj_map *map, fj_tag_type_t key_type, fj_tag_type_t value_type);

/** Frees the allocated memory. */
void fj_map_deinit(struct fj_map *map);

/**  */
fj_bool8_t fj_map_is_empty(struct fj_map const *map);

/** */
fj_bool8_t fj_map_has_allocated(struct fj_map const *map);

/** */
fj_err_t fj_map_set(struct fj_map *map, union fj_tag key, union fj_tag value);

/** Returns the pointer to value. Returns NULL if the key was not found. */
void *fj_map_get(struct fj_map const *map, union fj_tag key);

/** */
fj_err_t fj_map_remove(struct fj_map *map, union fj_tag key);

/** */
void fj_map_iter_init(struct fj_map_iter *iter, struct fj_map const *map);

/** */
fj_bool8_t fj_map_iter_finished(struct fj_map_iter const *iter);

/**
Returns true if the next element was successfully found.

:param element: Returns the next element or NULL.
*/
fj_bool8_t fj_map_iter_next(struct fj_map_iter *iter, struct fj_map_element **element);


#endif
