#ifndef FEJIX_IDLIST_H_
#define FEJIX_IDLIST_H_


#include <fejix/core/types.h>


/// Linear dynamic array of identifiers (`fj_id_t`).
struct fj_idlist {
    fj_id_t * elements;
    uint32_t length;
    uint32_t capacity;
};


struct fj_idlist * fj_idlist_new(void);

void fj_idlist_del(struct fj_idlist * list);

/// Returns `FJ_INTERNAL_FAIL` if the index is out of `[0; length]`.
fj_result_t fj_idlist_insert(
    struct fj_idlist * list,
    uint32_t index,
    fj_id_t item
);

/// Returns `FJ_INTERNAL_FAIL` if the index is out of `[0; length]`.
fj_result_t fj_idlist_remove(struct fj_idlist * list, uint32_t index);

/// Returns `FJ_OK` if the item was not found.
/// The only purpose of this function is to ensure that the given item is not
/// present on the list.
fj_result_t fj_idlist_remove_item(struct fj_idlist * list, fj_id_t item);

fj_result_t fj_idlist_push(struct fj_idlist * list, fj_id_t item);

fj_result_t fj_idlist_pop(struct fj_idlist * list);


#endif