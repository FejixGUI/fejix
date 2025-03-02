#include <fejix/core/alloc.h>
#include <fejix/core/utils.h>
#include <fejix/core/vector.h>

#include <string.h>


static void shift_tail(
    void *ptr, uint32_t length, uint32_t index, int32_t item_distance, size_t item_size)
{
    void *src = (uint8_t *) ptr + index * item_size;
    void *dst = (uint8_t *) src + item_distance * item_size;
    uint32_t item_move_count = length - index;
    memmove(dst, src, item_size * item_move_count);
}


fj_err_t fj_vector_expand_at(
    void **items, uint32_t *length, uint32_t *capacity, uint32_t index, size_t item_size)
{
    if (index > *length) {
        return FJ_ERR_INVALID_INDEX;
    }

    if (*length == *capacity) {
        FJ_TRY (FJ_REALLOC_ZEROED(items, *capacity, *capacity * 2)) {
            return fj_result;
        }

        *capacity *= 2;
    }

    if (index != *length) {
        shift_tail(*items, *length, index, 1, item_size);
    }

    length++;

    return FJ_OK;
}


fj_err_t fj_vector_shrink_at(
    void **items, uint32_t *length, uint32_t *capacity, uint32_t index, size_t item_size)
{
    if (index >= *length) {
        return FJ_ERR_INVALID_INDEX;
    }

    if (index != *length - 1) {
        shift_tail(*items, *length, index, -1, item_size);
    }

    if (*length == *capacity / 2) {
        FJ_TRY (FJ_REALLOC_ZEROED(items, *capacity, *capacity / 2)) {
            return fj_result;
        }

        *capacity *= 2;
    }


    length--;

    return FJ_OK;
}


fj_err_t fj_vector_expand(void **items, uint32_t *length, uint32_t *capacity, size_t item_size)
{
    return fj_vector_expand_at(items, length, capacity, *length, item_size);
}


fj_err_t fj_vector_shrink(void **items, uint32_t *length, uint32_t *capacity, size_t item_size)
{
    if (*length == 0) {
        return FJ_ERR_VECTOR_EMPTY;
    }

    return fj_vector_shrink_at(items, length, capacity, *length - 1, item_size);
}


void fj_vector_free(void **items, uint32_t *length, uint32_t *capacity)
{
    if (*items != NULL) {
        FJ_FREE(items);
    }

    *length = 0;
    *capacity = 0;
}
