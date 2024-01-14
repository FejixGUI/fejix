#include <fejix/vec.h>
#include <fejix/malloc.h>
#include <fejix/utils.h>

#include <string.h>


/** Small optimization to reduce the amount of allocations. */
static
uint32_t vec_get_min_capacity(struct fj_vec * vec)
{
    if (vec->item_size == 1) {
        return 8;
    } else if (vec->item_size < 256) {
        return 4;
    } else {
        return 1;
    }
}


static
uint32_t vec_get_capacity_to_grow(struct fj_vec * vec, uint32_t grow_count)
{
    uint32_t new_capacity = FJ_MAX(vec->capacity, 1);

    while (vec->length + grow_count > new_capacity) {
        new_capacity *= 2;
    }

    return FJ_MAX(new_capacity, vec_get_min_capacity(vec));
}


static
uint32_t vec_get_capacity_to_shrink(struct fj_vec * vec)
{
    uint32_t new_capacity = vec->capacity;

    while (vec->length < new_capacity/4) {
        new_capacity /= 2;
    }

    return FJ_MAX(new_capacity, vec_get_min_capacity(vec));
}


static
fj_err_t vec_set_capacity(struct fj_vec * vec, uint32_t capacity)
{
    return fj_realloc_uninit(&vec->items, capacity, vec->item_size);
}


fj_err_t fj_vec_ensure_reserved(struct fj_vec * vec, uint32_t reserved_items)
{
    uint32_t new_capacity = vec_get_capacity_to_grow(vec, reserved_items);
    return vec_set_capacity(vec, new_capacity);
}


static
fj_err_t vec_maybe_shrink(struct fj_vec * vec)
{
    uint32_t new_capacity = vec_get_capacity_to_shrink(vec);
    return vec_set_capacity(vec, new_capacity);
}


fj_err_t fj_vec_shrink_to_fit(struct fj_vec * vec)
{
    return vec_set_capacity(vec, vec->length);
}


static
void shift_items_to_grow(
    struct fj_vec * vec,
    uint32_t source_index,
    uint32_t item_distance
)
{
    uint8_t * src = fj_vec_offset(vec, source_index);
    uint8_t * dst = src + item_distance * vec->item_size;
    uint32_t item_move_count = vec->length - source_index;
    memmove(dst, src, item_move_count * vec->item_size);
}


static
void shift_items_to_shrink(
    struct fj_vec * vec,
    uint32_t source_index,
    uint32_t item_distance
)
{
    uint8_t * src = fj_vec_offset(vec, source_index);
    uint8_t * dst = src - item_distance * vec->item_size;
    uint32_t item_move_count = vec->length - source_index;
    memmove(dst, src, item_move_count * vec->item_size);
}


/*
static
fj_err_t vec_copy(struct fj_vec * dst, struct fj_vec * src)
{
    FJ_INIT_ERRORS

    FJ_TRY fj_vec_ensure_capacity(dst, src->capacity);

    if (FJ_FAILED) {
        return FJ_LAST_ERROR;
    }

    dst->length = src->length;

    size_t copy_size = src->length * src->item_size;
    memcpy(dst->items, src->items, copy_size);

    return FJ_OK;
}*/


void fj_vec_init(struct fj_vec * vec, size_t item_size)
{
    *vec = (struct fj_vec) { 0 };

    vec->item_size = item_size;
}


void fj_vec_deinit(struct fj_vec * vec)
{
    if (FJ_VEC_HOLDS_ITEMS(vec)) {
        fj_free(vec->items);
    }

    vec->items = NULL;
    vec->length = 0;
    vec->capacity = 0;
}


void fj_vec_copy_items(
    struct fj_vec * vec,
    void * items,
    uint32_t destination_index,
    uint32_t item_count
)
{
    uint8_t * destination = (uint8_t *) vec->items + destination_index;
    size_t copy_size = item_count * vec->item_size;
    memcpy(destination, items, copy_size);
}


fj_err_t fj_vec_insert_uninit(
    struct fj_vec * vec,
    uint32_t destination_index,
    uint32_t item_count
)
{
    FJ_INIT_ERRORS

    FJ_TRY fj_vec_ensure_reserved(vec, item_count);

    if (FJ_FAILED) {
        return FJ_LAST_ERROR;
    }

    if (!FJ_VEC_IS_EMPTY(vec) && destination_index != FJ_VEC_PUSH_INDEX(vec)) {
        shift_items_to_grow(vec, destination_index, item_count);
    }

    vec->length += item_count;

    return FJ_OK;
}


fj_err_t fj_vec_remove_items(
    struct fj_vec * vec,
    uint32_t start_index,
    uint32_t item_count
)
{
    if (
        !FJ_VEC_IS_EMPTY(vec)
        && start_index + item_count <= FJ_VEC_LAST_INDEX(vec)
    ) {
        shift_items_to_shrink(vec, start_index, item_count);
    }

    vec->length -= item_count;

    return vec_maybe_shrink(vec);
}


fj_err_t fj_vec_insert_items(
    struct fj_vec * vec,
    void * items,
    uint32_t destination_index,
    uint32_t item_count
)
{
    FJ_INIT_ERRORS

    FJ_TRY fj_vec_insert_uninit(vec, destination_index, item_count);

    if (FJ_FAILED) {
        return FJ_LAST_ERROR;
    }

    fj_vec_copy_items(vec, items, destination_index, item_count);

    return FJ_OK;
}


fj_err_t fj_vec_push_item(struct fj_vec * vec, void * item)
{
    return fj_vec_insert_items(vec, item, FJ_VEC_PUSH_INDEX(vec), 1);
}


fj_err_t fj_vec_pop_item(struct fj_vec * vec)
{
    return fj_vec_remove_items(vec, FJ_VEC_LAST_INDEX(vec), 1);
}


void * fj_vec_offset(struct fj_vec * vec, uint32_t index)
{
    if (index >= vec->length) {
        return NULL;
    }

    uint8_t * items = (uint8_t *) vec->items;
    return items + index * vec->item_size;
}


uint32_t fj_vec_find(
    struct fj_vec * vec,
    void * item,
    fj_vec_item_compararator_fn_t predicate
)
{
    return fj_vec_search(vec, item, predicate, 0, FJ_VEC_LAST_INDEX(vec));
}


uint32_t fj_vec_search(
    struct fj_vec * vec,
    void * item,
    fj_vec_item_compararator_fn_t predicate,
    uint32_t start_index,
    uint32_t end_index
)
{
    uint32_t index = start_index;
    int32_t step = start_index < end_index ? 1 : -1;

    for ( ; index < vec->length; index += step) {
        void * vec_item = fj_vec_offset(vec, index);

        if (predicate(vec_item, item) == true) {
            return index;
        }
    }

    return vec->length;
}
