#ifndef FEJIX_CORE_VEC_INCLUDED
#define FEJIX_CORE_VEC_INCLUDED


#include <fejix/core/base.h>


/** Dynamically-allocated linear array. */
struct fj_vec {
    /** NULL if the vector has not allocated. */
    void *items;

    /** Number of currently stored elements */
    uint32_t length;

    uint32_t _capacity;
    size_t _item_size;
};


/** The vector does not allocate any memory until any items get inserted. */
FJ_PUBLIC
void fj_vec_init(struct fj_vec *vec, size_t item_size);

/** Frees the memory if allocated. */
FJ_PUBLIC
void fj_vec_deinit(struct fj_vec *vec);

/**
    Resizes the vector to exactly the given capacity.
    If the new capacity is smaller that the current length, the length is trancated.
*/
FJ_PUBLIC
fj_err_t fj_vec_reallocate(struct fj_vec *vec, uint32_t capacity);

/**
    Ensures that the vector has at least the reserved elements.
    That is, the capacity is at least ``length + reserved_items``
*/
FJ_PUBLIC
fj_err_t fj_vec_reserve(struct fj_vec *vec, uint32_t capacity);

/** :returns: ``FJ_ERR_INVALID_INDEX``, ``FJ_ERR_CANNOT_ALLOCATE`` etc. */
FJ_PUBLIC
fj_err_t fj_vec_set(struct fj_vec const *vec, uint32_t index, void const *item);

/** :returns: ``FJ_ERR_INVALID_INDEX`` on failure. */
FJ_PUBLIC
fj_err_t fj_vec_get(struct fj_vec const *vec, uint32_t index, void *out_item);

/**
    :param index: The index between ``0`` and ``length-1``.
    :param item: The item to insert, copied. Must not be NULL.
    :returns: ``FJ_ERR_INVALID_INDEX``, ``FJ_ERR_CANNOT_ALLOCATE`` etc.
*/
FJ_PUBLIC
fj_err_t fj_vec_insert(struct fj_vec *vec, uint32_t index, void const *item);

/**
    :param index: The index between ``0`` and ``length-1``.
    :param out_item: If not NULL, returns the item before it is removed.
    :returns: ``FJ_ERR_INVALID_INDEX``, ``FJ_ERR_CANNOT_ALLOCATE`` etc.
*/
FJ_PUBLIC
fj_err_t fj_vec_remove(struct fj_vec *vec, uint32_t index, void *out_item);

/** Works like `insert`. Copies the item into the end of the list. */
FJ_PUBLIC
fj_err_t fj_vec_push(struct fj_vec *vec, void const *item);

/**
    Removes the last item of the list.
    :returns: ``FJ_ERR_NO_ITEMS`` etc.
*/
FJ_PUBLIC
fj_err_t fj_vec_pop(struct fj_vec *vec, void *out_item);


#endif
