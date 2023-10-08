#ifndef FEJIX_LIST_H_
#define FEJIX_LIST_H_


#include <fejix/base.h>


typedef fj_bool_t (*fj_comparator_t)(void * a, void * b);


/* Dynamically-allocated linear array. */
struct fj_list {
    void * items;
    size_t item_size;
    /* Number of currently stored elements */
    uint32_t length;
    /* Number of currently allocated elements */
    uint32_t capacity;
};


fj_err_t fj_list_new(struct fj_list * FJ_OUT* list, size_t item_size);

fj_err_t fj_list_clone(
    struct fj_list * FJ_OUT* destination,
    struct fj_list * source
);

void fj_list_del(struct fj_list * list);

/* Clones the item into the list.
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
    # Returns
    * The index of `item` if it is present.
    *`list->length` if the item was not found.*/
uint32_t fj_list_find(
    struct fj_list * list,
    void * item,
    fj_comparator_t predicate
);

/* Linearly searches for the item using the predicate function.
    The search begins with the start index and ends after the end index is
    reached. If `end_index < start_index`, then the search is done in backward
    direction.
    `start_index` and `end_index` are clamped to `0 .. list->length-1`
    # Returns
    * The index of `item` if it is present.
    *`list->length` if the item was not found. */
uint32_t fj_list_search(
    struct fj_list * list,
    void * item,
    fj_comparator_t predicate,
    uint32_t start_index,
    uint32_t end_index
);


#endif
