#ifndef FEJIX_MALLOC_H_
#define FEJIX_MALLOC_H_


#include <fejix/base.h>


/* Example: `FJ_ERR(FJ_MALLOC_FAILED)` */
#define FJ_MALLOC_FAILED "memory allocation failed"


/* Allocates an uninitialized block of memory.
    Works like `malloc`, but returns NULL if `size` is 0. */
fj_err_t fj_alloc_uninit(void FJ_NULLABLE* FJ_OUT* ptr, size_t size);

/* Allocates a block of memory initialized with zeros.
    Works like `calloc(1,)`, but returns NULL if `size` is 0. */
fj_err_t fj_alloc_zeroed(void FJ_NULLABLE* FJ_OUT* ptr, size_t size);

/* Works like `free`. If `ptr` is NULL, the behavior is undefined. */
void fj_free(void * ptr);

/* * If both `item_count` and `item_size` are greater than 0:
        - If `ptr` is not NULL:
            + reallocates the block, returns a new block
        - If `ptr` is NULL:
            + calls `fj_alloc_zeroed`, returns a new block
     * If either `item_count` or `item_size` is 0:
        - If `ptr` is not NULL:
            + calls `fj_free`, returns NULL
        - If `ptr` is NULL:
            + does nothing, returns NULL
    Works similarly to `realloc`, but with distinct `item_count` and
    `item_size`.
    Like `realloc`, this does NOT free the old block if fails to allocate
    a new one. */
fj_err_t fj_realloc(
    void FJ_NULLABLE* FJ_INOUT* ptr,
    uint32_t item_count,
    size_t item_size
);


#endif
