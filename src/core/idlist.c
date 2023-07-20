#include <fejix/idlist.h>
#include <fejix/utils.h>

#include <malloc.h>
#include <string.h>


/// Modifies the capacity, but leaves length as is
fj_result_t idlist_resize(fj_idlist_t * list, uint32_t capacity)
{
    list->elements = realloc(list->elements, capacity * sizeof(fj_id_t));
    
    if (list->elements == NULL) {
        list->capacity = 0;
        list->length = 0;

        return FJ_ALLOCATION_FAILED;
    }

    list->capacity = capacity;

    return FJ_OK;
}


fj_result_t idlist_grow(fj_idlist_t * list)
{
    if (list->length + 1 <= list->capacity) {
        return FJ_OK;
    }

    uint32_t new_capacity = fj_max_u32(1, list->capacity * 2);

    return idlist_resize(list, new_capacity);
}


fj_result_t idlist_shrink(fj_idlist_t * list)
{
    if (list->length >= list->capacity / 2) {
        return FJ_OK;
    }

    return idlist_resize(list, list->capacity / 2);
}


fj_idlist_t * fj_idlist_new()
{
    fj_idlist_t * list = calloc(1, sizeof(fj_idlist_t));

    return list;
}


void fj_idlist_del(fj_idlist_t * list)
{
    if (list->elements != NULL) {
        free(list->elements);
    }

    free(list);
}


fj_result_t fj_idlist_insert(fj_idlist_t * list, uint32_t index, fj_id_t elem)
{
    fj_result_t result = idlist_grow(list);
    if (result != FJ_OK) {
        return result;
    }

    list->length++;

    if (index > list->length) {
        return FJ_INTERNAL_ERROR;
    }

    if (index != list->length - 1) {
        fj_id_t * start = list->elements + index;
        uint32_t move_count = list->length - index;
        memmove(start, start + 1, move_count * sizeof(fj_id_t));
    }

    list->elements[index] = elem;

    return FJ_OK;
}


fj_result_t fj_idlist_remove(fj_idlist_t * list, uint32_t index)
{
    if (list->length == 0) {
        return FJ_OK;
    }

    if (index > list->length - 1) {
        return FJ_INTERNAL_ERROR;
    }

    if (index != list->length - 1) {
        fj_id_t * start = list->elements + index + 1;
        uint32_t move_count = list->length - index - 1;
        memmove(start, start - 1, move_count * sizeof(fj_id_t));
    }

    list->length--;

    return idlist_shrink(list);
}


fj_result_t fj_idlist_push(fj_idlist_t * list, fj_id_t element)
{
    return fj_idlist_insert(list, list->length, element);
}


fj_result_t fj_idlist_pop(fj_idlist_t * list)
{
    return fj_idlist_remove(list, list->length-1);
}