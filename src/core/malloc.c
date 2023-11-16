#include <fejix/malloc.h>
#include <fejix/utils.h>

#include <malloc.h>
#include <string.h>


fj_err_t fj_alloc_uninit(void * FJ_NULLABLE FJ_OUT * ptr, size_t size)
{
    if (size == 0) {
        ptr = NULL;
        return FJ_ERR("cannot allocate 0 bytes");
    }

    *ptr = malloc(size);

    if (*ptr == NULL) {
        return FJ_ERR(FJ_MALLOC_FAILED);
    }

    return FJ_OK;
}


fj_err_t fj_alloc_zeroed(void * FJ_NULLABLE FJ_OUT * ptr, size_t size)
{
    if (size == 0) {
        *ptr = NULL;
        return FJ_ERR("cannot allocate 0 bytes");
    }

    *ptr = calloc(1, size);

    if (*ptr == NULL) {
        return FJ_ERR(FJ_MALLOC_FAILED);
    }

    return FJ_OK;
}


void fj_free(void * ptr)
{
    free(ptr);
}


fj_err_t fj_realloc_uninit(
    void * FJ_NULLABLE FJ_ARRAY * FJ_INOUT ptr,
    uint32_t item_count,
    size_t item_size
)
{
    size_t size = item_count * item_size;

    if (item_count == 0 || item_size == 0) {
        if (ptr != NULL) {
            free(ptr);
        }

        *ptr = NULL;
        return FJ_OK;
    }

    if (*ptr == NULL) {
        return fj_alloc_uninit(ptr, size);
    }

    void * new_ptr = realloc(*ptr, size);

    if (new_ptr == NULL) {
        return FJ_ERR(FJ_MALLOC_FAILED);
    }

    *ptr = new_ptr;
    return FJ_OK;
}


fj_err_t fj_realloc_zeroed(
    void * FJ_NULLABLE FJ_ARRAY * FJ_INOUT ptr,
    uint32_t old_item_count,
    uint32_t new_item_count,
    size_t item_size
)
{
    void * old_ptr = *ptr;
    size_t old_size = old_item_count * item_size;
    size_t new_size = new_item_count * item_size;

    if (new_item_count == 0 || item_size == 0) {
        if (ptr != NULL) {
            free(ptr);
        }

        *ptr = NULL;
        return FJ_OK;
    }

    if (*ptr == NULL) {
        return fj_alloc_zeroed(ptr, new_size);
    }

    void * new_ptr = realloc(old_ptr, new_size);

    if (new_ptr == NULL) {
        return FJ_ERR(FJ_MALLOC_FAILED);
    }

    if (new_size > old_size) {
        memset((uint8_t *) new_ptr+old_size, 0, new_size-old_size);
    }

    *ptr = new_ptr;
    return FJ_OK;
}
