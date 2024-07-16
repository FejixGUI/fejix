#ifndef FEJIX_CORE_MALLOC_H_
#define FEJIX_CORE_MALLOC_H_


#include <fejix/core/base.h>


#define fj_alloc_uninit_auto(OBJECT_PTR_REF) \
    fj_alloc_uninit((void *)(OBJECT_PTR_REF), sizeof(**(OBJECT_PTR_REF)))

/** Example: int * x; fj_alloc_zeroed_auto(&x); */
#define fj_alloc_zeroed_auto(OBJECT_PTR_REF) \
    fj_alloc_zeroed((void *)(OBJECT_PTR_REF), sizeof(**(OBJECT_PTR_REF)))

#define fj_realloc_uninit_auto(ARRAY_PTR_REF, NEW_COUNT) \
    fj_realloc_uninit((void *)(ARRAY_PTR_REF), NEW_COUNT, sizeof(**(ARRAY_PTR_REF)))

/** Example: int x[]; fj_realloc_zeroed_auto(&x, 0, 10); */
#define fj_realloc_zeroed_auto(ARRAY_PTR_REF, OLD_COUNT, NEW_COUNT) \
    fj_realloc_zeroed((void *)(ARRAY_PTR_REF), OLD_COUNT, NEW_COUNT, sizeof(**(ARRAY_PTR_REF)))

#define fj_free_auto(OBJECT_PTR_REF) fj_free((void *)(OBJECT_PTR_REF))


/** Allocates an uninitialized block of memory.
    Works like `malloc`, but allocating 0 bytes is always an error.
    When this fails, sets `ptr` to NULL. */
fj_err_t fj_alloc_uninit(void */*? out*/ * ptr, size_t size);

/** Allocates a block of memory initialized with zeros.
    Works like `calloc(1,)`, but allocating 0 bytes is always an error.
    When this fails, sets `ptr` to NULL. */
fj_err_t fj_alloc_zeroed(void */*? out*/ * ptr, size_t size);

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
