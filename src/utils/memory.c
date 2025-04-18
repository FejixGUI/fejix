#include <fejix/utils/memory.h>

#include <malloc.h>
#include <stdio.h>
#include <string.h>


enum fj_error fj_alloc_uninit(void **out_ptr, size_t size)
{
    if (size == 0) {
        *out_ptr = NULL;
        return FJ_ERROR_INVALID_USAGE;
    }

    *out_ptr = malloc(size);

    if (*out_ptr == NULL) {
        return FJ_ERROR_OUT_OF_MEMORY;
    }

    return FJ_OK;
}


enum fj_error fj_alloc_zeroed(void **out_ptr, size_t size)
{
    if (size == 0) {
        *out_ptr = NULL;
        return FJ_ERROR_INVALID_USAGE;
    }

    *out_ptr = calloc(1, size);

    if (*out_ptr == NULL) {
        return FJ_ERROR_OUT_OF_MEMORY;
    }

    return FJ_OK;
}


enum fj_error fj_alloc_copied(void **out_ptr, void const *source, size_t size)
{
    enum fj_error e;
    e = fj_alloc_uninit(out_ptr, size);
    if (e)
        return e;

    memcpy(*out_ptr, source, size);

    return FJ_OK;
}


void fj_free(void **ptr)
{
    free(*ptr);
    *ptr = NULL;
}


enum fj_error fj_realloc_uninit(void **ptr, uint32_t items_length, size_t item_size)
{
    if (item_size == 0) {
        return FJ_ERROR_INVALID_USAGE;
    }

    if (items_length == 0) {
        if (ptr != NULL) {
            free((void *) ptr);
        }

        *ptr = NULL;
        return FJ_OK;
    }

    size_t size = items_length * item_size;

    if (*ptr == NULL) {
        return fj_alloc_uninit(ptr, size);
    }

    void *new_ptr = realloc(*ptr, size);

    if (new_ptr == NULL) {
        return FJ_ERROR_OUT_OF_MEMORY;
    }

    *ptr = new_ptr;
    return FJ_OK;
}


enum fj_error fj_realloc_zeroed(
    void **ptr, uint32_t old_items_length, uint32_t new_items_length, size_t item_size)
{
    if (item_size == 0) {
        return FJ_ERROR_INVALID_USAGE;
    }

    if (new_items_length == 0) {
        if (*ptr != NULL) {
            free((void *) *ptr);
        }

        *ptr = NULL;
        return FJ_OK;
    }

    void *old_ptr = *ptr;
    size_t old_size = old_items_length * item_size;
    size_t new_size = new_items_length * item_size;

    if (*ptr == NULL) {
        return fj_alloc_zeroed(ptr, new_size);
    }

    void *new_ptr = realloc(old_ptr, new_size);

    if (new_ptr == NULL) {
        return FJ_ERROR_OUT_OF_MEMORY;
    }

    if (new_size > old_size) {
        memset((uint8_t *) new_ptr + old_size, 0, new_size - old_size);
    }

    *ptr = new_ptr;
    return FJ_OK;
}


enum fj_error fj_string_clone(char const *str, char const **out_clone)
{
    enum fj_error e;

    size_t length = strlen(str);

    e = FJ_REALLOC_UNINIT(out_clone, length);

    if (e)
        return e;

    memcpy((void *) *out_clone, (void *) str, length);

    return FJ_OK;
}
