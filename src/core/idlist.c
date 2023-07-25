#include <fejix/core/idlist.h>
#include <fejix/core/utils.h>

#include <malloc.h>
#include <string.h>
#include <stdbool.h>


#define ELEMENT_SIZE sizeof(fj_id_t)


static bool is_empty(struct fj_idlist * list)
{
    return list->length == 0;
}

static uint32_t last_index(struct fj_idlist * list)
{
    return list->length - 1;
}

/// Modifies the capacity, but leaves length as is
static fj_result_t idlist_resize(struct fj_idlist * list, uint32_t capacity)
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


static fj_result_t idlist_grow(struct fj_idlist * list)
{
    if (list->length + 1 <= list->capacity) {
        return FJ_OK;
    }

    uint32_t new_capacity = fj_max(1, list->capacity * 2);

    return idlist_resize(list, new_capacity);
}


static fj_result_t idlist_shrink(struct fj_idlist * list)
{
    if (list->length >= list->capacity / 2) {
        return FJ_OK;
    }

    return idlist_resize(list, list->capacity / 2);
}


static void shift_elements_right(struct fj_idlist * list, uint32_t index)
{
    fj_id_t * start = list->elements + index;
    uint32_t move_count = list->length - index;
    memmove(start + 1, start, move_count * ELEMENT_SIZE);
}


static void shift_elements_left(struct fj_idlist * list, uint32_t index)
{
    fj_id_t * start = list->elements + index + 1;
    uint32_t move_count = list->length - index - 1;
    memmove(start - 1, start, move_count * ELEMENT_SIZE);
}


struct fj_idlist * fj_idlist_new(void)
{
    struct fj_idlist * list = calloc(1, sizeof(struct fj_idlist));

    return list;
}


void fj_idlist_del(struct fj_idlist * list)
{
    if (list->elements != NULL) {
        free(list->elements);
    }

    free(list);
}


fj_result_t fj_idlist_insert(
    struct fj_idlist * list,
    uint32_t index,
    fj_id_t elem
)
{
    if (index > last_index(list)+1) {
        return FJ_INTERNAL_FAIL;
    }

    fj_result_t result = idlist_grow(list);
    if (result != FJ_OK) {
        return result;
    }

    list->length++;

    if (!is_empty(list) && index != last_index(list)) {
        shift_elements_right(list, index);
    }

    list->elements[index] = elem;

    return FJ_OK;
}


fj_result_t fj_idlist_remove(struct fj_idlist * list, uint32_t index)
{
    if (is_empty(list) || index > last_index(list)) {
        return FJ_INTERNAL_FAIL;
    }

    if (index != last_index(list)) {
        shift_elements_left(list, index);
    }

    list->length--;

    return idlist_shrink(list);
}


fj_result_t fj_idlist_remove_item(struct fj_idlist * list, fj_id_t item)
{
    for (uint32_t i = 0; i < list->length; i++) {
        if (list->elements[i] == item) {
            return fj_idlist_remove(list, i);
        }
    }

    return FJ_OK;
}


fj_result_t fj_idlist_push(struct fj_idlist * list, fj_id_t element)
{
    return fj_idlist_insert(list, last_index(list)+1, element);
}


fj_result_t fj_idlist_pop(struct fj_idlist * list)
{
    return fj_idlist_remove(list, last_index(list));
}