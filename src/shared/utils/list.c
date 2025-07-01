#include "list.h"

#include <src/shared/utils/logging.h>
#include <src/shared/utils/math.h>
#include <src/shared/utils/memory.h>

#include <string.h>


static void shift_tail(
    void *ptr, uint32_t length, uint32_t source_index, uint32_t destination_index, size_t item_size)
{
    void *src = (uint8_t *) ptr + source_index * item_size;
    void *dst = (uint8_t *) ptr + destination_index * item_size;
    uint32_t moved_items_length = length - source_index;
    memmove(dst, src, item_size * moved_items_length);
}


enum fj_status fj_list_expand(
    void **items, uint32_t *length, uint32_t *capacity, size_t item_size, uint32_t index)
{
    enum fj_status s;

    if (index > *length) {
        FJ_ERROR("push index out of range");
        return FJ_INVALID_USAGE;
    }

    if (*length == *capacity) {
        uint32_t new_capacity = fj_u32_max(*capacity * 2, 1);
        s = fj_realloc_zeroed(items, *capacity, new_capacity, item_size);

        if (s)
            return s;

        *capacity = new_capacity;
    }

    if (index != *length) {
        shift_tail(*items, *length, index, index + 1, item_size);
    }

    *length += 1;

    return FJ_OK;
}


enum fj_status fj_list_shrink(
    void **items, uint32_t *length, uint32_t *capacity, size_t item_size, uint32_t index)
{
    enum fj_status s;

    if (index >= *length) {
        if (*length == 0) {
            FJ_ERROR("cannot remove from an empty list");
        } else {
            FJ_ERROR("remove index out of range");
        }

        return FJ_INVALID_USAGE;
    }

    if (index != *length - 1) {
        shift_tail(*items, *length, index + 1, index, item_size);
    }

    if (*length <= *capacity / 4) {
        uint32_t new_capacity = fj_u32_max(*capacity / 2, 1);
        s = fj_realloc_zeroed(items, *capacity, new_capacity, item_size);
        if (s)
            return s;

        *capacity = new_capacity;
    }


    *length -= 1;

    return FJ_OK;
}


void fj_list_free(void **items, uint32_t *length, uint32_t *capacity, size_t item_size)
{
    if (*items != NULL) {
        fj_free(items, *capacity * item_size);
    }

    *length = 0;
    *capacity = 0;
}
