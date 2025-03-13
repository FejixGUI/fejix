#include <fejix/utils/math.h>
#include <fejix/utils/memory.h>
#include <fejix/utils/vector.h>

#include <string.h>


static void shift_tail(
    void *ptr, uint32_t length, uint32_t source_index, uint32_t destination_index, size_t item_size)
{
    void *src = (uint8_t *) ptr + source_index * item_size;
    void *dst = (uint8_t *) ptr + destination_index * item_size;
    uint32_t moved_items_length = length - source_index;
    memmove(dst, src, item_size * moved_items_length);
}


fj_err_t fj_vector_expand_at(
    void **items, uint32_t *length, uint32_t *capacity, uint32_t index, size_t item_size)
{
    if (index > *length) {
        return FJ_ERR_INVALID_USAGE;
    }

    if (*length == *capacity) {
        uint32_t new_capacity = fj_u32_max(*capacity * 2, 1);
        FJ_TRY (fj_realloc_zeroed(items, *capacity, new_capacity, item_size)) {
            return fj_result;
        }

        *capacity *= 2;
    }

    if (index != *length) {
        shift_tail(*items, *length, index, index + 1, item_size);
    }

    *length += 1;

    return FJ_OK;
}


fj_err_t fj_vector_shrink_at(
    void **items, uint32_t *length, uint32_t *capacity, uint32_t index, size_t item_size)
{
    if (index >= *length) {
        return FJ_ERR_INVALID_USAGE;
    }

    if (index != *length - 1) {
        shift_tail(*items, *length, index + 1, index, item_size);
    }

    if (*length == *capacity / 2) {
        uint32_t new_capacity = fj_u32_max(*capacity / 2, 1);
        FJ_TRY (fj_realloc_zeroed(items, *capacity, new_capacity, item_size)) {
            return fj_result;
        }

        *capacity *= 2;
    }


    *length += 1;

    return FJ_OK;
}


void fj_vector_free(void **items, uint32_t *length, uint32_t *capacity)
{
    if (*items != NULL) {
        FJ_FREE(items);
    }

    *length = 0;
    *capacity = 0;
}
