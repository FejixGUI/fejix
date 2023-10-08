#include <fejix/malloc.h>
#include <fejix/utils.h>

#include <malloc.h>


fj_err_t fj_alloc_uninit(void FJ_OUT* * ptr, size_t size)
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


fj_err_t fj_alloc_zeroed(void FJ_OUT* * ptr, size_t size)
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


fj_err_t fj_realloc(void FJ_INOUT* * ptr, uint32_t item_count, size_t item_size)
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
        return fj_alloc_zeroed(ptr, size);
    }

    void * new_ptr = realloc(*ptr, size);

    if (new_ptr == NULL) {
        return FJ_ERR(FJ_MALLOC_FAILED);
    }

    *ptr = new_ptr;
    return FJ_OK;
}
