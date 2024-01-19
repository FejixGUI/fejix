#ifndef FEJIX_MALLOC_H_
#define FEJIX_MALLOC_H_


#include <fejix/base.h>


/** Allocates an uninitialized block of memory.
    Works like `malloc`, but allocating 0 bytes is always an error.
    When this fails, sets `ptr` to NULL. */
fj_err_t fj_alloc_uninit(void * FJ_NULLABLE FJ_OUT * ptr, size_t size);

/** Allocates a block of memory initialized with zeros.
    Works like `calloc(1,)`, but allocating 0 bytes is always an error.
    When this fails, sets `ptr` to NULL. */
fj_err_t fj_alloc_zeroed(void * FJ_NULLABLE FJ_OUT * ptr, size_t size);

/** Works like `free`. If `ptr` is NULL, the behavior is undefined. */
void fj_free(void * ptr);

/** Similar to `realloc`, but with distinct `item_count` and `item_size`.

    - `fj_realloc(NULL, 0, 0) = nothing`
    - `fj_realloc(NULL, x, y) = alloc_uninit(x*y)`
    - `fj_realloc(ptr, 0, 0) = free(ptr)`
    - `fj_realloc(ptr, x, y) = realloc_uninit(ptr, x*y)`

    In this scheme `realloc_uninit()` does not initialise new memory to zeroes,
    just like `fj_alloc_uninit`.

    If reallocation fails, this does not change the given pointer,
    does not free the old block and returns an error. */
fj_err_t fj_realloc_uninit(
    void * FJ_NULLABLE FJ_ARRAY * ptr,
    uint32_t item_count,
    size_t item_size
);


/** Similar to `fj_realloc_uninit`, but the allocated memory is initialised to
    zeroes. */
fj_err_t fj_realloc_zeroed(
    void * FJ_NULLABLE FJ_ARRAY * ptr,
    uint32_t old_item_count,
    uint32_t new_item_count,
    size_t item_size
);


#endif
