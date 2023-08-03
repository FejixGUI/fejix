#ifndef FEJIX_LIST_H_
#define FEJIX_LIST_H_


#include <fejix/core/base.h>


/// Dynamically-allocated linear array.
struct fj_list {
    fj_ptr_t elements;
    size_t element_size;
    uint32_t length;
    uint32_t capacity;
};


/// Returns NULL if an allocation error occurs.
struct fj_list * fj_list_new(size_t element_size);

/// Returns NULL if an allocation error occurs.
struct fj_list * fj_list_clone(struct fj_list * source);

void fj_list_del(struct fj_list * list);

/// Clones the item into the list.
/// Returns an error if the index is out of `[0; length]`.
fj_err_t fj_list_insert(struct fj_list * list, uint32_t index, fj_ptr_t item);

/// Returns an error if the index is out of `[0; length-1]`.
fj_err_t fj_list_remove(struct fj_list * list, uint32_t index);

/// Clones the item into the list.
fj_err_t fj_list_push(struct fj_list * list, fj_ptr_t item);

fj_err_t fj_list_pop(struct fj_list * list);

/// Returns NULL if `index` is out of bounds.
fj_ptr_t fj_list_get(struct fj_list * list, uint32_t index);

/// Searches the item by byte-wise comparison.
/// Returns the index of `item` if it is present.
/// Returns `list->length` if the item was not found.
/// This has `O(length)` complexity.
uint32_t fj_list_find(struct fj_list * list, fj_ptr_t item);

/// If the item is present on the list, removes it.
fj_err_t fj_list_exclude(struct fj_list * list, fj_ptr_t item);


#endif
