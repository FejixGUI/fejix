#ifndef FEJIX_VEC_H_
#define FEJIX_VEC_H_


#include <fejix/base.h>


/* Converts the pointer to the items from an untyped pointer to a types pointer.
    This is intended to be a faster way to access the items. */
#define FJ_VEC_OF(VEC, TYPE) ((TYPE *) (VEC)->items)

#define FJ_VEC_LAST_INDEX(VEC) ((VEC)->length - 1)

/** Evaluates to the index at which the insertion becomes pushing,
    i.e. no items should be shifted. */
#define FJ_VEC_PUSH_INDEX(VEC) ((VEC)->length)

#define FJ_VEC_IS_EMPTY(VEC) ((VEC)->length == 0)

#define FJ_VEC_HOLDS_ITEMS(VEC) ((VEC)->items != NULL)


typedef fj_bool_t (* fj_vec_item_compararator_fn_t)(void * a, void * b);


/** Dynamically-allocated linear array. */
struct fj_vec {
    /* Number of currently stored elements */
    uint32_t length;
    /* Number of currently allocated elements */
    uint32_t capacity;
    size_t item_size;
    void * FJ_ARRAY items;
};


/** The vector does not allocate any memory until any items get inserted. */
void fj_vec_init(struct fj_vec * vec, size_t item_size);

void fj_vec_deinit(struct fj_vec * vec);

/** Ensures that the vector has at least the reserved elements.
    That is, the capacity is at least `length + reserved_items`
    This may resize the vector. */
fj_err_t fj_vec_ensure_reserved(struct fj_vec * vec, uint32_t reserved_items);

/** Sets the capacity to the vector length. */
fj_err_t fj_vec_shrink_to_fit(struct fj_vec * vec);


/** Copies the items into the list.

    Does not check the index. */
void fj_vec_copy_items(
    struct fj_vec * vec,
    void * items,
    uint32_t destination_index,
    uint32_t item_count
);


/** Extends the vector at the given index by the given number of items and
    lives the space uninitialised.

    Does not check the index for the bounds.

    === EXAMPLE ===

    ```
    struct fj_vec v;
    fj_vec_init(&v, sizeof int);

    fj_vec_insert_uninit(&v, 0, 3);

    int * items = v->items;
    items[0] = 11;
    items[1] = 22;
    items[2] = 33;
    ```

    === RETURNS ===

    Returns an error if memory allocation fails. */
fj_err_t fj_vec_insert_uninit(
    struct fj_vec * vec,
    uint32_t destination_index,
    uint32_t item_count
);

/** Works like `fj_vec_insert_uninit`+`fj_vec_copy_items`. */
fj_err_t fj_vec_insert_items(
    struct fj_vec * vec,
    void * items,
    uint32_t destination_index,
    uint32_t item_count
);

/** Returns an error if memory allocation fails.
    Clamps `start_index` and `start_index+item_count` to the vector bounds. */
fj_err_t fj_vec_remove_items(
    struct fj_vec * vec,
    uint32_t start_index,
    uint32_t item_count
);

/** Returns the pointer to the items offset by the given index.
    Returns NULL if the index is out of range. */
void * FJ_NULLABLE FJ_ARRAY fj_vec_offset(
    struct fj_vec * vec,
    uint32_t start_index
);

/** Works like `insert`. Copies the item into the end of the list. */
fj_err_t fj_vec_push_item(struct fj_vec * vec, void * item);

/** Removes the last item of the list. */
fj_err_t fj_vec_pop_item(struct fj_vec * vec);

/** Linearly searches for the item using the predicate function.

    === ARGS ===

    - `start_index` - the index at which the search will start.
        Clamped to `0..length-1`.

    - `end_index` - the index at which the search will end.
        If `end_index < start_index`, the search will be performed in backward
        direction.
        Clamped to `0..length-1`.

    === RETURNS ===

    - The index of the item if it was found.
    - The length of the array if the item was not found. */
uint32_t fj_vec_search(
    struct fj_vec * vec,
    void * item,
    fj_vec_item_compararator_fn_t predicate,
    uint32_t start_index,
    uint32_t end_index
);


/** Linearly searches for the item using the predicate function.

    This is the same as:
    `fj_vec_search(vec, item, predicate, 0, vec->length-1)`.

    === RETURNS ===

    - The index of the item if it was found.
    - The length of the array if the item was not found. */
uint32_t fj_vec_find(
    struct fj_vec * vec,
    void * item,
    fj_vec_item_compararator_fn_t predicate
);


#endif
