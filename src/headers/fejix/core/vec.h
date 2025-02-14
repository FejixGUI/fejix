#ifndef FEJIX_CORE_VEC_INCLUDED
#define FEJIX_CORE_VEC_INCLUDED


#include <fejix/core/base.h>


/** Dynamically-allocated linear array. */
struct fj_vec {
    /** NULL if the vector has not allocated. */
    void *items;

    /** Number of currently stored elements */
    uint32_t length;

    /* Number of currently allocated elements */
    uint32_t _capacity;

    size_t _item_size;
};


/** The vector does not allocate any memory until any items get inserted. */
FJ_PUBLIC
void fj_vec_init(struct fj_vec *vec, size_t item_size);

/** */
FJ_PUBLIC
void fj_vec_deinit(struct fj_vec *vec);


/** Ensures that the vector's capacity is at least 1. */
FJ_PUBLIC
fj_err_t fj_vec_allocate(struct fj_vec *vec);

/**
    Resizes the vector to exactly the given capacity.
    If the new capacity is smaller that the current length, the length is trancated.
*/
FJ_PUBLIC
fj_err_t fj_vec_resize(struct fj_vec *vec, uint32_t capacity);

/**
    Ensures that the vector has at least the reserved elements.
    That is, the capacity is at least ``length + reserved_items``
*/
FJ_PUBLIC
fj_err_t fj_vec_resize_to_reserve(struct fj_vec *vec, uint32_t reserved_items);

/** Sets the capacity to the vector length. */
FJ_PUBLIC
fj_err_t fj_vec_resize_to_fit(struct fj_vec *vec);

FJ_PUBLIC
uint32_t fj_vec_get_last_index(struct fj_vec const *vec);

FJ_PUBLIC
void *fj_vec_offset(struct fj_vec const *vec, uint32_t offset_index);

FJ_PUBLIC
void *fj_vec_last_item(struct fj_vec const *vec);


/** Copies the items into the list. Does not check the index. */
FJ_PUBLIC
void fj_vec_replace(
    struct fj_vec *vec, void const *items, uint32_t destination_index, uint32_t item_count);

FJ_PUBLIC
fj_err_t fj_vec_insert_uninit(struct fj_vec *vec, uint32_t destination_index, uint32_t item_count);

FJ_PUBLIC
fj_err_t fj_vec_insert(
    struct fj_vec *vec, void const *items, uint32_t destination_index, uint32_t item_count);

/** Clamps `start_index` and `start_index+item_count` to `0..length`. */
FJ_PUBLIC
fj_err_t fj_vec_remove(struct fj_vec *vec, uint32_t start_index, uint32_t item_count);

/** Works like `insert`. Copies the item into the end of the list. */
FJ_PUBLIC
fj_err_t fj_vec_push(struct fj_vec *vec, void const *item);

/** Removes the last item of the list. */
FJ_PUBLIC
fj_err_t fj_vec_pop(struct fj_vec *vec);


#endif
