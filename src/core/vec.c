#include <fejix/core/alloc.h>
#include <fejix/core/utils.h>
#include <fejix/core/vec.h>

#include <string.h>


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


static uint32_t vec_get_capacity_to_grow(struct fj_vec *vec, uint32_t grow_count)
{
    return FJ_MAX(fj_u32_next_power_of_two(vec->length + grow_count), vec_get_min_capacity(vec));
}


static uint32_t vec_get_capacity_to_shrink(struct fj_vec *vec)
{
    return FJ_MAX(fj_u32_next_power_of_two(vec->length), vec_get_min_capacity(vec));
}


fj_err_t fj_vec_resize(struct fj_vec *vec, uint32_t capacity)
{
    if (vec->_capacity == capacity) {
        return FJ_OK;
    }

    FJ_TRY (fj_realloc_uninit(&vec->items, capacity, vec->_item_size)) {
        return fj_result;
    }

    vec->_capacity = capacity;
    vec->length = FJ_MIN(vec->length, vec->_capacity);

    return FJ_OK;
}


fj_err_t fj_vec_resize_to_reserve(struct fj_vec *vec, uint32_t reserved_items)
{
    uint32_t new_capacity = vec_get_capacity_to_grow(vec, reserved_items);
    return fj_vec_resize(vec, new_capacity);
}


fj_err_t fj_vec_allocate(struct fj_vec *vec)
{
    return fj_vec_resize_to_reserve(vec, 1);
}


fj_err_t fj_vec_resize_to_fit(struct fj_vec *vec)
{
    return fj_vec_resize(vec, vec->length);
}


uint32_t fj_vec_get_last_index(struct fj_vec const *vec)
{
    if (vec->length == 0) {
        return 0;
    }

    return vec->length - 1;
}

uint32_t fj_vec_get_push_index(struct fj_vec const *vec)
{
    return vec->length;
}

fj_bool8_t fj_vec_is_empty(struct fj_vec const *vec)
{
    return vec->length == 0;
}

fj_bool8_t fj_vec_has_allocated(struct fj_vec const *vec)
{
    return vec->items != NULL;
}

void *fj_vec_offset(struct fj_vec const *vec, uint32_t offset_index)
{
    if (offset_index >= vec->_capacity) {
        return NULL;
    }

    return (uint8_t *) vec->items + vec->_item_size * (size_t) offset_index;
}

void *fj_vec_last_item(struct fj_vec const *vec)
{
    return fj_vec_offset(vec, fj_vec_get_last_index(vec));
}


static fj_err_t vec_maybe_shrink(struct fj_vec *vec)
{
    uint32_t new_capacity = vec_get_capacity_to_shrink(vec);
    return fj_vec_resize(vec, new_capacity);
}


static void vec_shift_tail(struct fj_vec *vec, uint32_t start_index, int32_t shift_item_distance)
{
    uint8_t *src = fj_vec_offset(vec, start_index);
    uint8_t *dst = src + shift_item_distance * vec->_item_size;
    uint32_t item_move_count = vec->length - start_index;
    memmove(dst, src, vec->_item_size * item_move_count);
}


void fj_vec_init(struct fj_vec *vec, size_t item_size)
{
    *vec = (struct fj_vec) { ._item_size = item_size };
}


void fj_vec_deinit(struct fj_vec *vec)
{
    if (fj_vec_has_allocated(vec)) {
        FJ_FREE(&vec->items);
    }

    vec->length = 0;
    vec->_capacity = 0;
}


void fj_vec_replace(
    struct fj_vec *vec,
    void const *items,
    uint32_t destination_index,
    uint32_t item_count
)
{
    uint8_t *destination = fj_vec_offset(vec, destination_index);
    size_t copy_size = vec->_item_size * item_count;
    memcpy(destination, items, copy_size);
}


fj_err_t fj_vec_insert_uninit(struct fj_vec *vec, uint32_t destination_index, uint32_t item_count)
{
    FJ_TRY (fj_vec_resize_to_reserve(vec, item_count)) {
        return fj_result;
    }

    if (!fj_vec_is_empty(vec) && destination_index != fj_vec_get_push_index(vec)) {
        vec_shift_tail(vec, destination_index, (int32_t) item_count);
    }

    vec->length += item_count;

    return FJ_OK;
}


fj_err_t fj_vec_insert(
    struct fj_vec *vec,
    void const *items,
    uint32_t destination_index,
    uint32_t item_count
)
{
    FJ_TRY (fj_vec_insert_uninit(vec, destination_index, item_count)) {
        return fj_result;
    }

    fj_vec_replace(vec, items, destination_index, item_count);

    return FJ_OK;
}


fj_err_t fj_vec_remove(struct fj_vec *vec, uint32_t start_index, uint32_t item_count)
{
    if (start_index + item_count <= fj_vec_get_last_index(vec)) {
        vec_shift_tail(vec, start_index + item_count, -(int32_t) item_count);
    }

    vec->length -= item_count;

    return vec_maybe_shrink(vec);
}


fj_err_t fj_vec_push(struct fj_vec *vec, void const *item)
{
    return fj_vec_insert(vec, item, fj_vec_get_push_index(vec), 1);
}


fj_err_t fj_vec_pop(struct fj_vec *vec)
{
    return fj_vec_remove(vec, fj_vec_get_last_index(vec), 1);
}


fj_err_t fj_vec_push_front(struct fj_vec *vec, void const *item)
{
    return fj_vec_insert(vec, item, 0, 1);
}


fj_err_t fj_vec_pop_front(struct fj_vec *vec)
{
    return fj_vec_remove(vec, 0, 1);
}
