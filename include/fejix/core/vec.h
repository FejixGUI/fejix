#ifndef FEJIX_CORE_VEC_H_
#define FEJIX_CORE_VEC_H_


#include <fejix/core/base.h>


/** Dynamically-allocated linear array. */
struct fj_vec {
    void */*[]?*/ items;

    /* Number of currently stored elements */
    uint32_t length;

    /* Number of currently allocated elements */
    uint32_t capacity;

    size_t item_size;
};


/** The vector does not allocate any memory until any items get inserted. */
void fj_vec_init(
    struct fj_vec * vec,
    size_t item_size
);

void fj_vec_deinit(
    struct fj_vec * vec
);


/** If the new capacity is smaller that the current length, the length is
    trancated. */
fj_err_t fj_vec_resize(
    struct fj_vec * vec,
    uint32_t capacity
);

/** Ensures that the vector has at least the reserved elements.
    That is, the capacity is at least `length + reserved_items` */
fj_err_t fj_vec_resize_to_reserve(
    struct fj_vec * vec,
    uint32_t reserved_items
);

/** Sets the capacity to the vector length. */
fj_err_t fj_vec_resize_to_fit(
    struct fj_vec * vec
);


uint32_t fj_vec_get_last_index(
    struct fj_vec const * vec
);

uint32_t fj_vec_get_push_index(
    struct fj_vec const * vec
);

fj_bool32_t fj_vec_is_empty(
    struct fj_vec const * vec
);

fj_bool32_t fj_vec_has_allocated(
    struct fj_vec const * vec
);

void */*[]?*/ fj_vec_offset(
    struct fj_vec const * vec,
    uint32_t offset_index
);


/** Copies the items into the list.

    Does not check the index. */
void fj_vec_replace_items(
    struct fj_vec * vec,
    void * items,
    uint32_t destination_index,
    uint32_t item_count
);

fj_err_t fj_vec_insert_uninit(
    struct fj_vec * vec,
    uint32_t destination_index,
    uint32_t item_count
);

fj_err_t fj_vec_insert_items(
    struct fj_vec * vec,
    void * items,
    uint32_t destination_index,
    uint32_t item_count
);

/** Clamps `start_index` and `start_index+item_count` to `0..length`. */
fj_err_t fj_vec_remove_items(
    struct fj_vec * vec,
    uint32_t start_index,
    uint32_t item_count
);

/** Works like `insert`. Copies the item into the end of the list. */
fj_err_t fj_vec_push_item(
    struct fj_vec * vec,
    void * item
);

/** Removes the last item of the list. */
fj_err_t fj_vec_pop_item(
    struct fj_vec * vec
);


#endif
