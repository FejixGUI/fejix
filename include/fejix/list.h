#ifndef FEJIX_LIST_H_
#define FEJIX_LIST_H_


#include <fejix/base.h>

// TODO add initial size

typedef fj_bool_t (* fj_comparator_t)(void * a, void * b);


/* Dynamically-allocated linear array. */
struct fj_list {
    void * FJ_ARRAY items;
    size_t item_size;
    /* Number of currently stored elements */
    uint32_t length;
    /* Number of currently allocated elements */
    uint32_t capacity;
};


fj_err_t fj_list_new(
    struct fj_list * FJ_NULLABLE * FJ_OUT list,
    size_t item_size
);

fj_err_t fj_list_clone(
    struct fj_list * FJ_NULLABLE * FJ_OUT destination,
    struct fj_list * source
);

void fj_list_del(struct fj_list * list);

/* Clones the item into the list.

    === RETURNS ===

    Returns an error if the index is out of `[0; length]`. */
fj_err_t fj_list_insert(struct fj_list * list, uint32_t index, void * item);

/* Returns an error if the index is out of `[0; length-1]`. */
fj_err_t fj_list_remove(struct fj_list * list, uint32_t index);

/* Clones the item into the list. */
fj_err_t fj_list_push(struct fj_list * list, void * item);

fj_err_t fj_list_pop(struct fj_list * list);

/* Returns NULL if `index` is out of bounds. */
void * fj_list_get(struct fj_list * list, uint32_t index);

/* Linearly searches for the item using the predicate function.

    This is the same as:
    `fj_list_search(list, item, predicate, 0, list->length-1)`.

    === RETURNS ===

    - The index of the item if it was found.
    - The length of the array if the item was not found. */
uint32_t fj_list_find(
    struct fj_list * list,
    void * item,
    fj_comparator_t predicate
);

/* Linearly searches for the item using the predicate function.

    === ARGS ===

    - `start_index` - the index at which the search will start.
        Clamped to `0..length-1`.

    - `end_index` - the index at which the search will end.
        If `end_index < start_index`, the search will be performed in backwards
        direction.
        Clamped to `0..length-1`.

    === RETURNS ===

    - The index of the item if it was found.
    - The length of the array if the item was not found. */
uint32_t fj_list_search(
    struct fj_list * list,
    void * item,
    fj_comparator_t predicate,
    uint32_t start_index,
    uint32_t end_index
);


#endif
