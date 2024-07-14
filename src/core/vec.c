#include <fejix/core/vec.h>

#include <fejix/core/malloc.h>
#include <fejix/core/utils.h>

#include <string.h>


uint32_t fj_vec_get_last_index(struct fj_vec const * vec)
{
    return vec->length - 1;
}

uint32_t fj_vec_get_push_index(struct fj_vec const * vec)
{
    return vec->length;
}

fj_bool32_t fj_vec_is_empty(struct fj_vec const * vec)
{
    return vec->length == 0;
}

fj_bool32_t fj_vec_has_allocated(struct fj_vec const * vec)
{
    return vec->items != NULL;
}

void */*[]?*/ fj_vec_offset(
    struct fj_vec const * vec,
    uint32_t offset_index
)
{
    if (offset_index >= vec->capacity) {
        return NULL;
    }

    return (uint8_t *) vec->items + vec->item_size * offset_index;
}


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


fj_err_t fj_vec_resize(struct fj_vec * vec, uint32_t capacity)
{
    FJ_INIT_TRY

    if (vec->capacity == capacity) {
        return FJ_OK;
    }

    fj_try fj_realloc_uninit(&vec->items, capacity, vec->item_size);
    fj_else {
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


fj_err_t fj_vec_resize_to_fit(struct fj_vec * vec)
{
    return fj_vec_resize(vec, vec->length);
}


static
fj_err_t vec_maybe_shrink(struct fj_vec * vec)
{
    uint32_t new_capacity = vec_get_capacity_to_shrink(vec);
    return fj_vec_resize(vec, new_capacity);
}


static
void shift_items_for_insert(
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
void shift_items_for_remove(
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


void fj_vec_init(struct fj_vec * vec, size_t item_size)
{
    *vec = (struct fj_vec) {
        .item_size = item_size
    };
}


void fj_vec_deinit(struct fj_vec * vec)
{
    if (fj_vec_has_allocated(vec)) {
        fj_free_auto(&vec->items);
    }

    vec->length = 0;
    vec->capacity = 0;
}


void fj_vec_replace_items(
    struct fj_vec * vec,
    void const * items,
    uint32_t destination_index,
    uint32_t item_count
)
{
    uint8_t * destination = fj_vec_offset(vec, destination_index);
    size_t copy_size = item_count * vec->item_size;
    memcpy(destination, items, copy_size);
}


fj_err_t fj_vec_insert_uninit(
    struct fj_vec * vec,
    uint32_t destination_index,
    uint32_t item_count
)
{
    FJ_INIT_TRY

    fj_try fj_vec_resize_to_reserve(vec, item_count);
    fj_else {
        return fj_result;
    }

    if (
        !fj_vec_is_empty(vec)
        && destination_index != fj_vec_get_push_index(vec)
    ) {
        shift_items_for_insert(vec, destination_index, item_count);
    }

    vec->length += item_count;

    return FJ_OK;
}


fj_err_t fj_vec_insert_items(
    struct fj_vec * vec,
    void const * items,
    uint32_t destination_index,
    uint32_t item_count
)
{
    FJ_INIT_TRY

    fj_try fj_vec_insert_uninit(vec, destination_index, item_count);
    fj_else {
        return fj_result;
    }

    fj_vec_replace_items(vec, items, destination_index, item_count);

    return FJ_OK;
}


fj_err_t fj_vec_remove_items(
    struct fj_vec * vec,
    uint32_t start_index,
    uint32_t item_count
)
{
    if (start_index + item_count <= fj_vec_get_last_index(vec)) {
        shift_items_for_remove(vec, start_index+item_count, item_count);
    }

    vec->length -= item_count;

    return vec_maybe_shrink(vec);
}


fj_err_t fj_vec_push_item(struct fj_vec * vec, void const * item)
{
    return fj_vec_insert_items(vec, item, fj_vec_get_push_index(vec), 1);
}


fj_err_t fj_vec_pop_item(struct fj_vec * vec)
{
    return fj_vec_remove_items(vec, fj_vec_get_last_index(vec), 1);
}
