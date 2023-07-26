#include <fejix/core/list.h>
#include <fejix/core/utils.h>

#include <malloc.h>
#include <string.h>
#include <stdbool.h>


#define ELEMENT_SIZE sizeof(fj_id_t)


static bool is_empty(struct fj_list * list)
{
    return list->length == 0;
}

static uint32_t last_index(struct fj_list * list)
{
    return list->length - 1;
}

/// Modifies the capacity, but leaves length as is
static fj_result_t list_resize(struct fj_list * list, uint32_t capacity)
{
    list->elements = realloc(list->elements, capacity * ELEMENT_SIZE);
    
    if (list->elements == NULL) {
        list->capacity = 0;
        list->length = 0;

        return FJ_MALLOC_FAIL;
    }

    list->capacity = capacity;

    return FJ_OK;
}


static fj_result_t list_grow(struct fj_list * list)
{
    if (list->length + 1 <= list->capacity) {
        return FJ_OK;
    }

    uint32_t new_capacity = fj_max(1, list->capacity * 2);

    return list_resize(list, new_capacity);
}


static fj_result_t list_shrink(struct fj_list * list)
{
    if (list->length >= list->capacity / 2) {
        return FJ_OK;
    }

    return list_resize(list, list->capacity / 2);
}


static void shift_elements_right(struct fj_list * list, uint32_t index)
{
    fj_id_t * start = list->elements + index;
    uint32_t move_count = list->length - index;
    memmove(start + 1, start, move_count * ELEMENT_SIZE);
}


static void shift_elements_left(struct fj_list * list, uint32_t index)
{
    fj_id_t * start = list->elements + index + 1;
    uint32_t move_count = list->length - index - 1;
    memmove(start - 1, start, move_count * ELEMENT_SIZE);
}


struct fj_list * fj_list_new(void)
{
    struct fj_list * list = calloc(1, sizeof(struct fj_list));

    return list;
}


struct fj_list * fj_list_clone(struct fj_list * source)
{
    struct fj_list * list = fj_list_new();

    if (list == NULL) {
        return NULL;
    }

    if (list_resize(list, source->capacity) != FJ_OK) {
        fj_list_del(list);
        return NULL;
    }

    memcpy(list->elements, source->elements, source->length * ELEMENT_SIZE);

    return list;
}


void fj_list_del(struct fj_list * list)
{
    if (list->elements != NULL) {
        free(list->elements);
    }

    free(list);
}


fj_result_t fj_list_insert(struct fj_list * list, uint32_t index, fj_id_t elem)
{
    if (index > last_index(list)+1) {
        return FJ_INTERNAL_FAIL;
    }

    if (list_grow(list) != FJ_OK) {
        return FJ_MALLOC_FAIL;
    }

    list->length++;

    if (!is_empty(list) && index != last_index(list)) {
        shift_elements_right(list, index);
    }

    list->elements[index] = elem;

    return FJ_OK;
}


fj_result_t fj_list_remove(struct fj_list * list, uint32_t index)
{
    if (is_empty(list) || index > last_index(list)) {
        return FJ_INTERNAL_FAIL;
    }

    if (index != last_index(list)) {
        shift_elements_left(list, index);
    }

    list->length--;

    return list_shrink(list);
}


fj_result_t fj_list_push(struct fj_list * list, fj_id_t element)
{
    return fj_list_insert(list, last_index(list) + 1, element);
}


fj_result_t fj_list_pop(struct fj_list * list)
{
    return fj_list_remove(list, last_index(list));
}


uint32_t fj_list_find(struct fj_list * list, fj_id_t item)
{
    uint32_t i;
    for (i = 0; i < list->length; i++) {
        if (list->elements[i] == item) {
            break;
        }
    }

    return i;
}


fj_result_t fj_list_include(struct fj_list * list, fj_id_t item)
{
    if (fj_list_find(list, item) != list->length) {
        return FJ_OK;
    }

    return fj_list_push(list, item);
}


fj_result_t fj_list_exclude(struct fj_list * list, fj_id_t item)
{
    uint32_t index = fj_list_find(list, item);

    if (index == list->length) {
        return FJ_OK;
    }

    return fj_list_remove(list, index);
}