#ifndef FEJIX_LIST_H_
#define FEJIX_LIST_H_


#include <fejix/base.h>


typedef fj_bool_t (*fj_comparator_t)(fj_ptr_t a, fj_ptr_t b);


/* Dynamically-allocated linear array. */
struct fj_list {
    fj_ptr_t elements;
    size_t element_size;
    /* Number of currently stored elements */
    uint32_t length;
    /* Size of `elements` in element sizes */
    uint32_t capacity;
};


/* Returns NULL if an allocation error occurs. */
struct fj_list * fj_list_new(size_t element_size);

/* Returns NULL if an allocation error occurs. */
struct fj_list * fj_list_clone(struct fj_list * source);

void fj_list_del(struct fj_list * list);

/* Clones the item into the list.
    Returns an error if the index is out of `[0; length]`. */
fj_err_t fj_list_insert(struct fj_list * list, uint32_t index, fj_ptr_t item);

/* Returns an error if the index is out of `[0; length-1]`. */
fj_err_t fj_list_remove(struct fj_list * list, uint32_t index);

/* Clones the item into the list. */
fj_err_t fj_list_push(struct fj_list * list, fj_ptr_t item);

fj_err_t fj_list_pop(struct fj_list * list);

/* Returns NULL if `index` is out of bounds. */
fj_ptr_t fj_list_get(struct fj_list * list, uint32_t index);

/* Linearly searches for the item using the predicate function.
    This is analogous to `fj_list_search(list, item, predicate, 0, true)`.
    # Returns
    * The index of `item` if it is present.
    *`list->length` if the item was not found.*/
uint32_t fj_list_find(
    struct fj_list * list,
    fj_ptr_t item,
    fj_comparator_t predicate
);

/* Linearly searches for the item using the predicate function.
    The search starts from the given start index and goes forward if `forward`
    is true or backward otherwise.
    # Returns
    * The index of `item` if it is present.
    *`list->length` if the item was not found.*/
uint32_t fj_list_search(
    struct fj_list * list,
    fj_ptr_t item,
    fj_comparator_t predicate,
    uint32_t start_index,
    fj_bool_t forward
);


#endif
