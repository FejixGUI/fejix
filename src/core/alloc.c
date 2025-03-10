#include <fejix/core/alloc.h>
#include <fejix/core/utils.h>

#include <malloc.h>
#include <stdio.h>
#include <string.h>


fj_err_t fj_alloc_uninit(void **out_ptr, size_t size)
{
    if (size == 0) {
        *out_ptr = NULL;
        return FJ_ERR_INVALID_ALLOCATION;
    }

    *out_ptr = malloc(size);

    if (*out_ptr == NULL) {
        return FJ_ERR_CANNOT_ALLOCATE;
    }

    return FJ_OK;
}


fj_err_t fj_alloc_zeroed(void **out_ptr, size_t size)
{
    if (size == 0) {
        *out_ptr = NULL;
        return FJ_ERR_INVALID_ALLOCATION;
    }

    *out_ptr = calloc(1, size);

    if (*out_ptr == NULL) {
        return FJ_ERR_CANNOT_ALLOCATE;
    }

    return FJ_OK;
}


fj_err_t fj_alloc_copied(void **out_ptr, void const *source, size_t size)
{
    FJ_TRY (fj_alloc_uninit(out_ptr, size)) {
        return fj_result;
    }

    memcpy(*out_ptr, source, size);

    return FJ_OK;
}


void fj_free(void **ptr)
{
    free(*ptr);
    *ptr = NULL;
}


fj_err_t fj_realloc_uninit(void **ptr, uint32_t item_count, size_t item_size)
{
    size_t size = item_count * item_size;

    if (item_count == 0 || item_size == 0) {
        if (ptr != NULL) {
            free((void *) ptr);
        }

        *ptr = NULL;
        return FJ_OK;
    }

    if (*ptr == NULL) {
        return fj_alloc_uninit(ptr, size);
    }

    void *new_ptr = realloc(*ptr, size);

    if (new_ptr == NULL) {
        return FJ_ERR_CANNOT_ALLOCATE;
    }

    *ptr = new_ptr;
    return FJ_OK;
}


fj_err_t fj_realloc_zeroed(
    void **ptr, uint32_t old_item_count, uint32_t new_item_count, size_t item_size)
{
    void *old_ptr = *ptr;
    size_t old_size = old_item_count * item_size;
    size_t new_size = new_item_count * item_size;

    if (new_item_count == 0 || item_size == 0) {
        if (ptr != NULL) {
            free((void *) ptr);
        }

        *ptr = NULL;
        return FJ_OK;
    }

    if (*ptr == NULL) {
        return fj_alloc_zeroed(ptr, new_size);
    }

    void *new_ptr = realloc(old_ptr, new_size);

    if (new_ptr == NULL) {
        return FJ_ERR_CANNOT_ALLOCATE;
    }

    if (new_size > old_size) {
        memset((uint8_t *) new_ptr + old_size, 0, new_size - old_size);
    }

    *ptr = new_ptr;
    return FJ_OK;
}
