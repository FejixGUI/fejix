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
    list->elements = fj_realloc(list->elements, capacity, list->element_size);

    if (list->elements == NULL) {
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


static void shift_elements_right(struct fj_list * list, uint32_t src_index)
{
    uint8_t * src = fj_list_get(list, src_index);
    uint8_t * dst = src + list->element_size;
    uint32_t move_count = list->length - src_index;
    memmove(dst, src, move_count * list->element_size);
}


static void shift_elements_left(struct fj_list * list, uint32_t dst_index)
{
    uint8_t * dst = fj_list_get(list, dst_index);
    uint8_t * src = dst + list->element_size;
    uint32_t move_count = list->length - dst_index - 1;
    memmove(dst, src, move_count * list->element_size);
}


struct fj_list * fj_list_new(size_t element_size)
{
    struct fj_list * list = fj_alloc_zeroed(sizeof *list);
    list->element_size = element_size;

    return list;
}


struct fj_list * fj_list_clone(struct fj_list * source)
{
    struct fj_list * list = fj_list_new(source->element_size);
    if (list == NULL) {
        return NULL;
    }

    if (list_resize(list, source->capacity) != FJ_OK) {
        fj_list_del(list);
        return NULL;
    }

    list->length = source->length;

    size_t copy_size = source->length * source->element_size;
    memcpy(list->elements, source->elements, copy_size);

    return list;
}


void fj_list_del(struct fj_list * list)
{
    if (list->elements != NULL) {
        fj_free(list->elements);
    }

    fj_free(list);
}


fj_err_t fj_list_insert(struct fj_list * list, uint32_t index, fj_ptr_t item)
{
    if (index > last_index(list)+1) {
        return FJ_ERR("cannot insert to list (index out of range)");
    }

    if (list_grow(list) != FJ_OK) {
        return FJ_ERR(FJ_MALLOC_FAILED);
    }

    list->length++;

    if (!is_empty(list) && index != last_index(list)) {
        shift_elements_right(list, index);
    }

    fj_ptr_t dst = fj_list_get(list, index);
    memcpy(dst, item, list->element_size);

    return FJ_OK;
}


fj_err_t fj_list_remove(struct fj_list * list, uint32_t index)
{
    if (is_empty(list) || index > last_index(list)) {
        return FJ_ERR("cannot remove from list (index out of range)");
    }

    if (index != last_index(list)) {
        shift_elements_left(list, index);
    }

    list->length--;

    return list_shrink(list);
}


fj_err_t fj_list_push(struct fj_list * list, fj_ptr_t item)
{
    return fj_list_insert(list, last_index(list) + 1, item);
}


fj_err_t fj_list_pop(struct fj_list * list)
{
    return fj_list_remove(list, last_index(list));
}


fj_ptr_t fj_list_get(struct fj_list * list, uint32_t index)
{
    if (index >= list->length) {
        return NULL;
    }

    uint8_t * elements = (uint8_t *) list->elements;
    return elements + index * list->element_size;
}


uint32_t fj_list_find(
    struct fj_list * list,
    fj_ptr_t item,
    fj_comparator_t predicate
)
{
    return fj_list_search(list, item, predicate, 0, true);
}


uint32_t fj_list_search(
    struct fj_list * list,
    fj_ptr_t item,
    fj_comparator_t predicate,
    uint32_t start_index,
    fj_bool_t forward
)
{
    uint32_t index = start_index;
    int32_t step = forward ? 1 : -1;

    for ( ; index < list->length; index += step) {
        fj_ptr_t list_item = fj_list_get(list, index);

        if (predicate(list_item, item) == true) {
            break;
        }
    }

    return index;
}
