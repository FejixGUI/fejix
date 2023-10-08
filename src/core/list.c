#include <fejix/list.h>
#include <fejix/malloc.h>
#include <fejix/utils.h>

#include <string.h>


static fj_bool_t is_empty(struct fj_list * list)
{
    return list->length == 0;
}


static uint32_t last_index(struct fj_list * list)
{
    return list->length - 1;
}


/* Modifies the capacity, but leaves length as is */
static fj_err_t list_resize(struct fj_list * list, uint32_t capacity)
{
    FJ_TRY_INIT

    FJ_TRY fj_realloc(&list->items, capacity, list->item_size);

    if (FJ_FAILED) {
        list->capacity = 0;
        list->length = 0;
        return FJ_ERR(FJ_MALLOC_FAILED);
    }

    list->capacity = capacity;

    return FJ_OK;
}


static fj_err_t list_grow(struct fj_list * list)
{
    if (list->length + 1 <= list->capacity) {
        return FJ_OK;
    }

    uint32_t new_capacity = fj_u32_max(1, list->capacity * 2);

    return list_resize(list, new_capacity);
}


static fj_err_t list_shrink(struct fj_list * list)
{
    if (list->length >= list->capacity / 2) {
        return FJ_OK;
    }

    return list_resize(list, list->capacity / 2);
}


static void shift_items_right(struct fj_list * list, uint32_t src_index)
{
    uint8_t * src = fj_list_get(list, src_index);
    uint8_t * dst = src + list->item_size;
    uint32_t move_count = list->length - src_index;
    memmove(dst, src, move_count * list->item_size);
}


static void shift_items_left(struct fj_list * list, uint32_t dst_index)
{
    uint8_t * dst = fj_list_get(list, dst_index);
    uint8_t * src = dst + list->item_size;
    uint32_t move_count = list->length - dst_index - 1;
    memmove(dst, src, move_count * list->item_size);
}


static fj_err_t list_copy(struct fj_list * dst, struct fj_list * src)
{
    FJ_TRY_INIT;

    FJ_TRY list_resize(dst, src->capacity);

    if (FJ_FAILED) {
        return FJ_LAST_ERROR;
    }

    dst->length = src->length;

    size_t copy_size = src->length * src->item_size;
    memcpy(dst->items, src->items, copy_size);

    return FJ_OK;
}


fj_err_t fj_list_new(
    struct fj_list FJ_NULLABLE* FJ_OUT* list,
    size_t item_size
)
{
    FJ_TRY_INIT;

    FJ_TRY fj_alloc_zeroed((void **) list, sizeof **list);

    if (FJ_FAILED) {
        return FJ_LAST_ERROR;
    }

    (*list)->item_size = item_size;

    return FJ_OK;
}


fj_err_t fj_list_clone(
    struct fj_list FJ_NULLABLE* FJ_OUT* destination,
    struct fj_list * source
)
{
    FJ_TRY_INIT;

    FJ_TRY fj_list_new(destination, source->item_size);

    if (FJ_FAILED) {
        return FJ_LAST_ERROR;
    }

    FJ_TRY list_copy(*destination, source);

    if (FJ_FAILED) {
        fj_list_del(*destination);
        return FJ_LAST_ERROR;
    }

    return FJ_OK;
}


void fj_list_del(struct fj_list * list)
{
    if (list->items != NULL) {
        fj_free(list->items);
    }

    fj_free(list);
}


fj_err_t fj_list_insert(struct fj_list * list, uint32_t index, void * item)
{
    if (index > last_index(list)+1) {
        return FJ_ERR("cannot insert to list (index out of range)");
    }

    if (list_grow(list) != FJ_OK) {
        return FJ_ERR(FJ_MALLOC_FAILED);
    }

    list->length++;

    if (!is_empty(list) && index != last_index(list)) {
        shift_items_right(list, index);
    }

    void * dst = fj_list_get(list, index);
    memcpy(dst, item, list->item_size);

    return FJ_OK;
}


fj_err_t fj_list_remove(struct fj_list * list, uint32_t index)
{
    if (is_empty(list) || index > last_index(list)) {
        return FJ_ERR("cannot remove from list (index out of range)");
    }

    if (index != last_index(list)) {
        shift_items_left(list, index);
    }

    list->length--;

    return list_shrink(list);
}


fj_err_t fj_list_push(struct fj_list * list, void * item)
{
    return fj_list_insert(list, last_index(list) + 1, item);
}


fj_err_t fj_list_pop(struct fj_list * list)
{
    return fj_list_remove(list, last_index(list));
}


void * fj_list_get(struct fj_list * list, uint32_t index)
{
    if (index >= list->length) {
        return NULL;
    }

    uint8_t * items = (uint8_t *) list->items;
    return items + index * list->item_size;
}


uint32_t fj_list_find(
    struct fj_list * list,
    void * item,
    fj_comparator_t predicate
)
{
    return fj_list_search(list, item, predicate, 0, true);
}


uint32_t fj_list_search(
    struct fj_list * list,
    void * item,
    fj_comparator_t predicate,
    uint32_t start_index,
    uint32_t end_index
)
{
    start_index = fj_u32_min(start_index, last_index(list));
    end_index = fj_u32_min(end_index, last_index(list));

    uint32_t index = start_index;
    int32_t step = start_index < end_index ? 1 : -1;

    for ( ; index < list->length; index += step) {
        void * list_item = fj_list_get(list, index);

        if (predicate(list_item, item) == true) {
            return index;
        }
    }

    return last_index(list);
}
