#include "memory.h"

#include <src/shared/utils/logging.h>

#include <malloc.h>
#include <stdio.h>
#include <string.h>


void *default_callback(void *pointer, size_t old_size, size_t new_size)
{
    (void) old_size;

    if (pointer == NULL)
        return malloc(new_size);

    if (new_size > 0)
        return realloc(pointer, new_size);

    free(pointer);
    return NULL;
}

void *(*fj_alloc_callback)(void *pointer, size_t old_size, size_t new_size) = default_callback;


enum fj_status fj_alloc_uninit(void **out_ptr, size_t size)
{
    if (size == 0) {
        *out_ptr = NULL;
        FJ_ERROR("failed to allocate 0 bytes");
        return FJ_INVALID_USAGE;
    }

    *out_ptr = fj_alloc_callback(NULL, 0, size);

    if (*out_ptr == NULL) {
        return FJ_OUT_OF_MEMORY;
    }

    return FJ_OK;
}


enum fj_status fj_alloc_zeroed(void **out_ptr, size_t size)
{
    if (size == 0) {
        *out_ptr = NULL;
        FJ_ERROR("failed to allocate 0 bytes");
        return FJ_INVALID_USAGE;
    }

    *out_ptr = fj_alloc_callback(NULL, 0, size);

    if (*out_ptr == NULL) {
        return FJ_OUT_OF_MEMORY;
    }

    memset(*out_ptr, 0, size);

    return FJ_OK;
}


enum fj_status fj_alloc_copied(void **out_ptr, void const *source, size_t size)
{
    enum fj_status s;

    s = fj_alloc_uninit(out_ptr, size);
    if (s)
        return s;

    memcpy(*out_ptr, source, size);

    return FJ_OK;
}


void fj_free(void **ptr, size_t size)
{
    if (ptr == NULL) {
        FJ_ERROR("failed to free NULL");
    }

    fj_alloc_callback(*ptr, size, 0);
    *ptr = NULL;
}


enum fj_status fj_realloc_uninit(void **ptr, size_t old_length, size_t new_length, size_t item_size)
{
    if (item_size == 0) {
        FJ_ERROR("realloc item size is 0");
        return FJ_INVALID_USAGE;
    }

    if (new_length == old_length) {
        return FJ_OK;
    }

    void *new_ptr = fj_alloc_callback(*ptr, old_length * item_size, new_length * item_size);

    if (new_ptr == NULL) {
        return FJ_OUT_OF_MEMORY;
    }

    *ptr = new_ptr;
    return FJ_OK;
}

enum fj_status fj_realloc_zeroed(void **ptr, size_t old_length, size_t new_length, size_t item_size)
{
    enum fj_status s = fj_realloc_uninit(ptr, old_length, new_length, item_size);

    if (s)
        return s;

    if (new_length > old_length) {
        memset((uint8_t *) *ptr + old_length * item_size, 0, (new_length - old_length) * item_size);
    }

    return FJ_OK;
}

enum fj_status fj_alloc_string_copied(char const **out_clone, char const *source)
{
    enum fj_status s;

    size_t length = strlen(source);

    s = FJ_REALLOC_UNINIT(out_clone, 0, length);

    if (s)
        return s;

    memcpy((void *) *out_clone, (void *) source, length);

    return FJ_OK;
}
