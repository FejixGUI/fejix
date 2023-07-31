#ifndef FEJIX_LIST_H_
#define FEJIX_LIST_H_


#include <fejix/core/base.h>


/// Linear dynamic array of identifiers (`fj_id_t`).
/// This can also be used as an ordered set with the `fj_list_include()` and
/// `fj_list_exclude()` methods.
struct fj_list {
    fj_id_t * elements;
    uint32_t length;
    uint32_t capacity;
};


/// Returns NULL if `fj_err_malloc` occurs.
struct fj_list * fj_list_new(void);

/// Returns NULL if `fj_err_malloc` occurs.
struct fj_list * fj_list_clone(struct fj_list * source);

void fj_list_del(struct fj_list * list);

/// Returns `FJ_INTERNAL_FAIL` if the index is out of `[0; length]`.
fj_err_t fj_list_insert(struct fj_list * list, uint32_t index, fj_id_t item);

/// Returns `FJ_INTERNAL_FAIL` if the index is out of `[0; length-1]`.
fj_err_t fj_list_remove(struct fj_list * list, uint32_t index);

fj_err_t fj_list_push(struct fj_list * list, fj_id_t item);

fj_err_t fj_list_pop(struct fj_list * list);

/// Returns the index of `item` if it is present.
/// Returns `list->length` if the item was not found.
/// This has `O(length)` complexity.
uint32_t fj_list_find(struct fj_list * list, fj_id_t item);

/// Ensures that `item` is uniquely present on the list.
/// Returns `FJ_OK` if `item` already exists.
/// This has `O(length)` complexity.
fj_err_t fj_list_include(struct fj_list * list, fj_id_t item);

/// Ensures that `item` is not present on the list.
/// Returns `fj_ok` if `item` was not found.
/// This has `O(length)` complexity.
fj_err_t fj_list_exclude(struct fj_list * list, fj_id_t item);


#endif