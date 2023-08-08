#include <fejix/core/malloc.h>
#include <fejix/core/utils.h>

#include <malloc.h>


fj_ptr_t fj_alloc_uninit(size_t size)
{
    if (size == 0) {
        return NULL;
    }

    return malloc(size);
}


fj_ptr_t fj_alloc_zeroed(size_t size)
{
    if (size == 0) {
        return NULL;
    }

    return calloc(1, size);
}


void fj_free(fj_ptr_t ptr)
{
    free(ptr);
}


fj_ptr_t fj_realloc(fj_ptr_t ptr, uint32_t element_count, size_t element_size)
{
    size_t size = element_count * element_size;

    if (element_count == 0 || element_size == 0) {
        if (ptr != NULL) {
            free(ptr);
        }

        return NULL;
    }

    if (ptr == NULL) {
        return fj_alloc_zeroed(size);
    }

    return realloc(ptr, size);
}
