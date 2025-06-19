#include <fejix/utils/logging.h>
#include <fejix/utils/memory.h>

#include <malloc.h>
#include <stdio.h>
#include <string.h>


void *default_callback(void *pointer, size_t old_size, size_t new_size, size_t alignment)
{
    (void) old_size;
    (void) alignment;

    if (pointer == NULL)
        return malloc(new_size);

    if (new_size > 0)
        return realloc(pointer, new_size);

    free(pointer);
    return NULL;
}

void *(*fj_allocation_callback)(void *pointer, size_t old_size, size_t new_size, size_t alignment)
    = default_callback;


enum fj_status fj_alloc_uninit(void **out_ptr, size_t size, size_t alignment)
{
    if (size == 0) {
        *out_ptr = NULL;
        FJ_ERROR("failed to allocate 0 bytes");
        return FJ_STATUS_INVALID_USAGE;
    }

    *out_ptr = fj_allocation_callback(NULL, 0, size, alignment);

    if (*out_ptr == NULL) {
        return FJ_STATUS_OUT_OF_MEMORY;
    }

    return FJ_STATUS_OK;
}


enum fj_status fj_alloc_zeroed(void **out_ptr, size_t size, size_t alignment)
{
    if (size == 0) {
        *out_ptr = NULL;
        FJ_ERROR("failed to allocate 0 bytes");
        return FJ_STATUS_INVALID_USAGE;
    }

    *out_ptr = fj_allocation_callback(NULL, 0, size, alignment);

    if (*out_ptr == NULL) {
        return FJ_STATUS_OUT_OF_MEMORY;
    }

    memset(*out_ptr, 0, size);

    return FJ_STATUS_OK;
}


enum fj_status fj_alloc_copied(void **out_ptr, void const *source, size_t size, size_t alignment)
{
    enum fj_status s;

    s = fj_alloc_uninit(out_ptr, size, alignment);
    if (s)
        return s;

    memcpy(*out_ptr, source, size);

    return FJ_STATUS_OK;
}


void fj_free(void **ptr, size_t size, size_t alignment)
{
    if (ptr == NULL) {
        FJ_ERROR("failed to free NULL");
    }

    fj_allocation_callback(*ptr, size, 0, alignment);
    *ptr = NULL;
}


enum fj_status fj_realloc_uninit(
    void **ptr, size_t old_length, size_t new_length, size_t item_size, size_t item_alignment)
{
    if (item_size == 0) {
        return FJ_STATUS_INVALID_USAGE;
    }

    if (new_length == old_length) {
        return FJ_STATUS_OK;
    }

    void *new_ptr = fj_allocation_callback(
        *ptr, old_length * item_size, new_length * item_size, item_alignment);

    if (new_ptr == NULL) {
        return FJ_STATUS_OUT_OF_MEMORY;
    }

    *ptr = new_ptr;
    return FJ_STATUS_OK;
}

enum fj_status fj_realloc_zeroed(
    void **ptr, size_t old_length, size_t new_length, size_t item_size, size_t item_alignment)
{
    enum fj_status s = fj_realloc_uninit(ptr, old_length, new_length, item_size, item_alignment);

    if (s)
        return s;

    if (new_length > old_length) {
        memset((uint8_t *) *ptr + old_length * item_size, 0, (new_length - old_length) * item_size);
    }

    return FJ_STATUS_OK;
}

enum fj_status fj_alloc_string_copied(char const **out_clone, char const *source)
{
    enum fj_status s;

    size_t length = strlen(source);

    s = FJ_REALLOC_UNINIT(out_clone, 0, length);

    if (s)
        return s;

    memcpy((void *) *out_clone, (void *) source, length);

    return FJ_STATUS_OK;
}
