#include <fejix/core/vec.h>

#include <fejix/core/alloc.h>
#include <fejix/core/utils.h>

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
    return FJ_MAX(fj_u32_next_power_of_two(vec->length + grow_count), vec_get_min_capacity(vec));
}


static
uint32_t vec_get_capacity_to_shrink(struct fj_vec * vec)
{
    return FJ_MAX(fj_u32_next_power_of_two(vec->length), vec_get_min_capacity(vec));

}


fj_err_t fj_vec_resize(struct fj_vec * vec, uint32_t capacity)
{
    if (vec->capacity == capacity) {
        return FJ_OK;
    }

    FJ_TRY(fj_realloc_uninit(&vec->items, capacity, vec->item_size)) {
        return fj_result;
    }

    vec->capacity = capacity;
    vec->length = FJ_MIN(vec->length, vec->capacity);

    return FJ_OK;
}


fj_err_t fj_vec_resize_to_reserve(struct fj_vec * vec, uint32_t reserved_items)
{
    uint32_t new_capacity = vec_get_capacity_to_grow(vec, reserved_items);
    return fj_vec_resize(vec, new_capacity);
}


fj_err_t fj_vec_allocate(struct fj_vec * vec)
{
    return fj_vec_resize_to_reserve(vec, 1);
}


fj_err_t fj_vec_resize_to_fit(struct fj_vec * vec)
{
    return fj_vec_resize(vec, vec->length);
}


static
fj_err_t vec_maybe_shrink(struct fj_vec * vec)
{
    size_t new_capacity = vec_get_capacity_to_shrink(vec);
    return fj_vec_resize(vec, new_capacity);
}


static
void shift_items_for_insert(struct fj_vec * vec, size_t source_index, size_t item_distance)
{
    uint8_t * src = fj_vec_offset(vec, source_index);
    uint8_t * dst = src + item_distance * vec->item_size;
    size_t item_move_count = vec->length - source_index;
    memmove(dst, src, item_move_count * vec->item_size);
}


static
void shift_items_for_remove(struct fj_vec * vec, size_t source_index, size_t item_distance)
{
    uint8_t * src = fj_vec_offset(vec, source_index);
    uint8_t * dst = src - item_distance * vec->item_size;
    size_t item_move_count = vec->length - source_index;
    memmove(dst, src, item_move_count * vec->item_size);
}


void fj_vec_init(struct fj_vec * vec, size_t item_size)
{
    *vec = (struct fj_vec) {
        .item_size = item_size
    };
}


void fj_vec_deinit(struct fj_vec * vec)
{
    if (fj_vec_has_allocated(vec)) {
        FJ_FREE(&vec->items);
    }

    vec->length = 0;
    vec->capacity = 0;
}


void fj_vec_replace(
    struct fj_vec * vec,
    void const * items,
    uint32_t destination_index,
    uint32_t item_count
)
{
    uint8_t * destination = fj_vec_offset(vec, destination_index);
    uint32_t copy_size = item_count * vec->item_size;
    memcpy(destination, items, copy_size);
}


fj_err_t fj_vec_insert_uninit(struct fj_vec * vec, uint32_t destination_index, uint32_t item_count)
{
    FJ_TRY(fj_vec_resize_to_reserve(vec, item_count)) {
        return fj_result;
    }

    if (!fj_vec_is_empty(vec) && destination_index != fj_vec_get_push_index(vec)) {
        shift_items_for_insert(vec, destination_index, item_count);
    }

    vec->length += item_count;

    return FJ_OK;
}


fj_err_t fj_vec_insert(
    struct fj_vec * vec,
    void const * items,
    uint32_t destination_index,
    uint32_t item_count
)
{
    FJ_TRY(fj_vec_insert_uninit(vec, destination_index, item_count)) {
        return fj_result;
    }

    fj_vec_replace(vec, items, destination_index, item_count);

    return FJ_OK;
}


fj_err_t fj_vec_remove(struct fj_vec * vec, uint32_t start_index, uint32_t item_count)
{
    if (start_index + item_count <= fj_vec_get_last_index(vec)) {
        shift_items_for_remove(vec, start_index+item_count, item_count);
    }

    vec->length -= item_count;

    return vec_maybe_shrink(vec);
}


fj_err_t fj_vec_push(struct fj_vec * vec, void const * item)
{
    return fj_vec_insert(vec, item, fj_vec_get_push_index(vec), 1);
}


fj_err_t fj_vec_pop(struct fj_vec * vec)
{
    return fj_vec_remove(vec, fj_vec_get_last_index(vec), 1);
}


fj_err_t fj_vec_push_front(struct fj_vec * vec, void const * item)
{
    return fj_vec_insert(vec, item, 0, 1);
}


fj_err_t fj_vec_pop_front(struct fj_vec * vec)
{
    return fj_vec_remove(vec, 0, 1);
}
