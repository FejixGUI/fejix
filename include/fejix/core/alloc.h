#ifndef FEJIX_CORE_ALLOC_H_
#define FEJIX_CORE_ALLOC_H_


#include <fejix/core/base.h>


/** Example: int * x; FJ_ALLOC_UNINIT(&x); *x = 3; */
#define FJ_ALLOC_UNINIT(OBJECT_PTR_REF) \
    (fj_alloc_uninit((void *)(OBJECT_PTR_REF), sizeof(**(OBJECT_PTR_REF))))

/** Example: int * x; FJ_ALLOC_ZEROED(&x); *x = 3; */
#define FJ_ALLOC_ZEROED(OBJECT_PTR_REF) \
    (fj_alloc_zeroed((void *)(OBJECT_PTR_REF), sizeof(**(OBJECT_PTR_REF))))

#define FJ_ALLOC_COPIED(OBJECT_PTR_REF, SOURCE) \
    (fj_alloc_copied((void *)(OBJECT_PTR_REF), (void *)(SOURCE), sizeof(**(OBJECT_PTR_REF))))

/** Example: int * x; FJ_ARRALLOC_UNINIT(&x, 10); x[9] = 3; */
#define FJ_ARRALLOC_UNINIT(ARRAY_PTR_REF, COUNT) \
    (fj_alloc_uninit((void *)(ARRAY_PTR_REF), sizeof(**(ARRAY_PTR_REF)) * (COUNT)))

/** Example: int * x; FJ_ARRALLOC_ZEROED(&x, 10); x[9] = 3; */
#define FJ_ARRALLOC_ZEROED(ARRAY_PTR_REF, COUNT) \
    (fj_alloc_zeroed((void *)(ARRAY_PTR_REF), sizeof(**(ARRAY_PTR_REF)) * (COUNT)))

/** Example: int * x; FJ_REALLOC_UNINIT(&x, 10); x[9] = 3; */
#define FJ_REALLOC_UNINIT(ARRAY_PTR_REF, NEW_COUNT) \
    (fj_realloc_uninit((void *)(ARRAY_PTR_REF), NEW_COUNT, sizeof(**(ARRAY_PTR_REF))))

/** Example: int * x; FJ_REALLOC_ZEROED(&x, 0, 10); x[9] = 3; */
#define FJ_REALLOC_ZEROED(ARRAY_PTR_REF, OLD_COUNT, NEW_COUNT) \
    (fj_realloc_zeroed((void *)(ARRAY_PTR_REF), OLD_COUNT, NEW_COUNT, sizeof(**(ARRAY_PTR_REF))))

#define FJ_FREE(OBJECT_PTR_REF) (fj_free((void *)(OBJECT_PTR_REF)))


/** Allocates an uninitialized block of memory.
    Works like `malloc`, but allocating 0 bytes is always an error.
    When this fails, sets `ptr` to NULL. */
fj_err_t fj_alloc_uninit(void */*? out*/ * ptr, size_t size);

/** Allocates a block of memory initialized with zeros.
    Works like `calloc(1,)`, but allocating 0 bytes is always an error.
    When this fails, sets `ptr` to NULL. */
fj_err_t fj_alloc_zeroed(void */*? out*/ * ptr, size_t size);

fj_err_t fj_alloc_copied(void */*? out*/ * ptr, void const * source, size_t size);

/** Frees a block of memory.
    This takes a reference to a pointer, frees the memory behind the pointer
    and assigns the pointer to NULL.
    If the pointer behind the reference is NULL, the behavior is undefined. */
void fj_free(void * * ptr);

/** Reallocates a block of memory for an array of items.

    - `fj_realloc_uninit(NULL, 0, y) = nothing`
    - `fj_realloc_uninit(NULL, x, y) = alloc_uninit(x*y)`
    - `fj_realloc_uninit(ptr,  0, y) = free(ptr)`
    - `fj_realloc_uninit(ptr,  x, y) = realloc_uninit(ptr, x*y)`

    If reallocation fails, this does not change the given pointer,
    does not free the old block and returns an error. */
fj_err_t fj_realloc_uninit(void */*[]?*/ * ptr, uint32_t item_count, size_t item_size);


/** Similar to `fj_realloc_uninit`, but all items of the array are always
    initialised to zeroes. */
fj_err_t fj_realloc_zeroed(
    void */*[]?*/ * ptr,
    uint32_t old_item_count,
    uint32_t new_item_count,
    size_t item_size
);


#endif
