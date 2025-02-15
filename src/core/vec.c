#include <fejix/core/alloc.h>
#include <fejix/core/utils.h>
#include <fejix/core/vec.h>

#include <string.h>


static void *vec_offset(struct fj_vec const *vec, uint32_t offset_index)
{
    if (offset_index >= vec->_capacity) {
        return NULL;
    }

    return (uint8_t *) vec->items + vec->_item_size * (size_t) offset_index;
}


/** Small optimization to reduce the amount of allocations for small vector sizes. */
static uint32_t vec_get_min_capacity(struct fj_vec *vec)
{
    if (vec->_item_size == 1) {
        return 8;
    } else if (vec->_item_size < 256) {
        return 4;
    } else {
        return 1;
    }
}


static uint32_t vec_get_grow_capacity(struct fj_vec *vec, uint32_t grow_count)
{
    return fj_u32_max(
        fj_u32_next_power_of_two(vec->length + grow_count), vec_get_min_capacity(vec));
}


static uint32_t vec_get_shrink_capacity(struct fj_vec *vec)
{
    return fj_u32_max(fj_u32_next_power_of_two(vec->length), vec_get_min_capacity(vec));
}


static fj_err_t vec_shrink(struct fj_vec *vec)
{
    uint32_t new_capacity = vec_get_shrink_capacity(vec);
    return fj_vec_reallocate(vec, new_capacity);
}


static void vec_shift_tail(struct fj_vec *vec, uint32_t index, int32_t item_distance)
{
    void *src = vec_offset(vec, index);
    void *dst = (uint8_t *) src + item_distance * vec->_item_size;
    uint32_t item_move_count = vec->length - index;
    memmove(dst, src, vec->_item_size * item_move_count);
}


void fj_vec_init(struct fj_vec *vec, size_t item_size)
{
    *vec = (struct fj_vec) { ._item_size = item_size };
}


void fj_vec_deinit(struct fj_vec *vec)
{
    if (vec->items != NULL) {
        FJ_FREE(&vec->items);
    }

    vec->length = 0;
    vec->_capacity = 0;
}


fj_err_t fj_vec_reallocate(struct fj_vec *vec, uint32_t capacity)
{
    if (vec->_capacity == capacity) {
        return FJ_OK;
    }

    FJ_TRY (fj_realloc_uninit(&vec->items, capacity, vec->_item_size)) {
        return fj_result;
    }

    vec->_capacity = capacity;
    vec->length = fj_u32_min(vec->length, vec->_capacity);

    return FJ_OK;
}


fj_err_t fj_vec_reserve(struct fj_vec *vec, uint32_t reserved_items)
{
    return fj_vec_reallocate(vec, vec_get_grow_capacity(vec, reserved_items));
}


fj_err_t fj_vec_set(struct fj_vec const *vec, uint32_t index, void const *item)
{
    if (index >= vec->length) {
        return FJ_ERR_INVALID_INDEX;
    }

    memcpy(vec_offset(vec, index), item, vec->_item_size);
    return FJ_OK;
}


fj_err_t fj_vec_get(struct fj_vec const *vec, uint32_t index, void *out_item)
{
    if (index >= vec->length) {
        return FJ_ERR_INVALID_INDEX;
    }

    memcpy(out_item, vec_offset(vec, index), vec->_item_size);
    return FJ_OK;
}


fj_err_t fj_vec_insert(struct fj_vec *vec, uint32_t index, void const *item)
{
    if (index > vec->length) {
        return FJ_ERR_INVALID_INDEX;
    }

    FJ_TRY (fj_vec_reserve(vec, 1)) {
        return fj_result;
    }

    if (index != vec->length) {
        vec_shift_tail(vec, index, 1);
    }

    vec->length++;

    memcpy(vec_offset(vec, index), item, vec->_item_size);
    return FJ_OK;
}


fj_err_t fj_vec_remove(struct fj_vec *vec, uint32_t index, void *out_item)
{
    if (index >= vec->length) {
        return FJ_ERR_INVALID_INDEX;
    }

    if (out_item != NULL) {
        fj_vec_get(vec, index, out_item);
    }

    if (index != vec->length - 1) {
        vec_shift_tail(vec, index + 1, -1);
    }

    vec->length--;

    return vec_shrink(vec);
}


fj_err_t fj_vec_push(struct fj_vec *vec, void const *item)
{
    return fj_vec_insert(vec, vec->length, item);
}


fj_err_t fj_vec_pop(struct fj_vec *vec, void *out_item)
{
    return fj_vec_remove(vec, vec->length - 1, out_item);
}
