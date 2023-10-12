#ifndef FEJIX_MALLOC_H_
#define FEJIX_MALLOC_H_


#include <fejix/base.h>


/* Example: `FJ_ERR(FJ_MALLOC_FAILED)` */
#define FJ_MALLOC_FAILED "memory allocation failed"


/* Allocates an uninitialized block of memory.
    Works like `malloc`, but returns NULL if `size` is 0. */
fj_err_t fj_alloc_uninit(void * FJ_NULLABLE * FJ_OUT ptr, size_t size);

/* Allocates a block of memory initialized with zeros.
    Works like `calloc(1,)`, but returns NULL if `size` is 0. */
fj_err_t fj_alloc_zeroed(void * FJ_NULLABLE * FJ_OUT ptr, size_t size);

/* Works like `free`. If `ptr` is NULL, the behavior is undefined. */
void fj_free(void * ptr);

/* Similar to `realloc`, but with distinct `item_count` and `item_size`.

    - `fj_realloc(NULL, 0, 0) = nothing`
    - `fj_realloc(NULL, x, y) = fj_alloc_zeroed(x*y)`
    - `fj_realloc(ptr, 0, 0) = fj_free(ptr)`
    - `fj_realloc(ptr, x, y) = fj_realloc(ptr, x*y)`

    If reallocation fails, this does not change the given pointer,
    does not free the old block and returns an error. */
fj_err_t fj_realloc(
    void * FJ_NULLABLE FJ_ARRAY * FJ_INOUT ptr,
    uint32_t item_count,
    size_t item_size
);


#endif
