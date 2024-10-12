#ifndef FEJIX_CORE_VEC_H_
#define FEJIX_CORE_VEC_H_


#include <fejix/core/base.h>


/** Dynamically-allocated linear array. */
struct fj_vec {
    /** NULL if the vector has not allocated. */
    void *items;

    /** Number of currently stored elements */
    uint32_t length;

    /** Number of currently allocated elements */
    uint32_t capacity;

    size_t item_size;
};


FJ_EXTERN_C_BEGIN


/** The vector does not allocate any memory until any items get inserted. */
void fj_vec_init(struct fj_vec *vec, size_t item_size);

/** */
void fj_vec_deinit(struct fj_vec *vec);


/** Ensures that the vector's capacity is at least 1. */
fj_err_t fj_vec_allocate(struct fj_vec *vec);

/** Resizes the vector to exactly the given capacity.
    If the new capacity is smaller that the current length, the length is trancated. */
fj_err_t fj_vec_resize(struct fj_vec *vec, uint32_t capacity);

/** Ensures that the vector has at least the reserved elements.
    That is, the capacity is at least `length + reserved_items` */
fj_err_t fj_vec_resize_to_reserve(struct fj_vec *vec, uint32_t reserved_items);

/** Sets the capacity to the vector length. */
fj_err_t fj_vec_resize_to_fit(struct fj_vec *vec);


static inline uint32_t fj_vec_get_last_index(struct fj_vec const *vec)
{
    if (vec->length == 0) {
        return 0;
    }

    return vec->length - 1;
}

static inline uint32_t fj_vec_get_push_index(struct fj_vec const *vec)
{
    return vec->length;
}

static inline fj_bool8_t fj_vec_is_empty(struct fj_vec const *vec)
{
    return vec->length == 0;
}

static inline fj_bool8_t fj_vec_has_allocated(struct fj_vec const *vec)
{
    return vec->items != NULL;
}

static inline void *fj_vec_offset(struct fj_vec const *vec, uint32_t offset_index)
{
    if (offset_index >= vec->capacity) {
        return NULL;
    }

    return (uint8_t *) vec->items + vec->item_size * (size_t) offset_index;
}

static inline void *fj_vec_last_item(struct fj_vec const *vec)
{
    return fj_vec_offset(vec, fj_vec_get_last_index(vec));
}


/** Copies the items into the list. Does not check the index. */
void fj_vec_replace(
    struct fj_vec *vec,
    void const *items,
    uint32_t destination_index,
    uint32_t item_count
);

fj_err_t fj_vec_insert_uninit(struct fj_vec *vec, uint32_t destination_index, uint32_t item_count);

fj_err_t fj_vec_insert(
    struct fj_vec *vec,
    void const *items,
    uint32_t destination_index,
    uint32_t item_count
);

/** Clamps `start_index` and `start_index+item_count` to `0..length`. */
fj_err_t fj_vec_remove(struct fj_vec *vec, uint32_t start_index, uint32_t item_count);

/** Works like `insert`. Copies the item into the end of the list. */
fj_err_t fj_vec_push(struct fj_vec *vec, void const *item);

/** Removes the last item of the list. */
fj_err_t fj_vec_pop(struct fj_vec *vec);

/** Pushed the item into the front (beginning) of the vector. */
fj_err_t fj_vec_push_front(struct fj_vec *vec, void const *item);

/** Pops an item from the front (beginning) of the vector. */
fj_err_t fj_vec_pop_front(struct fj_vec *vec);


FJ_EXTERN_C_END

#endif
