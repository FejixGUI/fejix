/** Contains convenient memory allocation functions and macros.

    Usage examples:

    ```
    int *x;

    FJ_ALLOC_UNINIT(&x);
    *x = 123;
    FJ_FREE(&x); // safely frees and nulls x
    assert(x == NULL);

    FJ_ALLOC(&x);
    assert(*x == 0);
    FJ_FREE(&x);

    int num = 123;
    FJ_ALLOC_COPIED(&x, &num);
    assert(*x == num);
    FJ_FREE(&x);

    FJ_REALLOC_UNINIT(&x, 10); // new size
    for (int i=0; i<10; i++) x[i] = 123;
    FJ_FREE(&x);

    FJ_REALLOC_ZEROED(&x, 0, 10); // old size, new size
    for (int i=0; i<10; i++) assert(x[i] == 0);
    FJ_REALLOC_ZEROED(&x, 10, 20);
    for (int i=10; i<20; i++) assert(x[i] == 0);
    FJ_FREE(&x);
    ```
*/

#ifndef FEJIX_COMMON_MEMORY_H_
#define FEJIX_COMMON_MEMORY_H_


#include <fejix/base.h>


#define FJ_ALLOC(...) (FJ_ALLOC_ZEROED(__VA_ARGS__))

#define FJ_ALLOC_ZEROED(OUT_OBJECT_PTR) \
    (fj_alloc_zeroed((void **) (OUT_OBJECT_PTR), sizeof(**(OUT_OBJECT_PTR))))

#define FJ_ALLOC_UNINIT(OUT_OBJECT_PTR) \
    (fj_alloc_uninit((void **) (OUT_OBJECT_PTR), sizeof(**(OUT_OBJECT_PTR))))

#define FJ_ALLOC_COPIED(OUT_OBJECT_PTR, SOURCE) \
    (fj_alloc_copied((void **) (OUT_OBJECT_PTR), (void *) (SOURCE), sizeof(**(OUT_OBJECT_PTR))))

#define FJ_REALLOC_UNINIT(ARRAY_PTR, OLD_LENGTH, NEW_LENGTH) \
    (fj_realloc_uninit((void **) (ARRAY_PTR), OLD_LENGTH, NEW_LENGTH, sizeof(**(ARRAY_PTR))))

#define FJ_REALLOC_ZEROED(ARRAY_PTR, OLD_LENGTH, NEW_LENGTH) \
    (fj_realloc_zeroed((void **) (ARRAY_PTR), (OLD_LENGTH), (NEW_LENGTH), sizeof(**(ARRAY_PTR))))

#define FJ_FREE(OBJECT_PTR) (fj_free((void **) (OBJECT_PTR), sizeof(**(OBJECT_PTR))))


/**
    Allocates an uninitialized block of memory.
    Works like `malloc`, but allocating 0 bytes is always an error.

    Returns a new pointer or NULL on failure.
*/
fj_err fj_alloc_uninit(void **out_ptr, size_t size);

/**
    Allocates a block of memory initialized with zeros.
    Works like `calloc(1,)`, but allocating 0 bytes is always an error.

    \returns a new pointer or NULL on failure.
*/
fj_err fj_alloc_zeroed(void **out_ptr, size_t size);

/**
    Allocates a block of memory initialised with bytes from `source`.

    \returns a new pointer or NULL on failure.
*/
fj_err fj_alloc_copied(void **out_ptr, void const *source, size_t size);

/**
    Frees a block of memory.

    References the pointer to be freed, always returns NULL.
*/
void fj_free(void **ptr, size_t size);

/**
    Reallocates a block of memory for an array of items.

    If reallocation fails, this does not change the given pointer and does not free the old block.

    * `realloc(NULL, 0, 0, item_size) = nothing`
    * `realloc(NULL, 0, x, item_size) = malloc(x*item_size)`
    * `realloc(ptr, x, 0, item_size) = free(ptr)`
    * `realloc(ptr, x, y, item_size) = realloc(y*item_size)`

    \returns Into `ptr`: a new pointer on success, does not change the pointer on failure.
*/
fj_err fj_realloc_uninit(void **ptr, size_t old_length, size_t new_length, size_t item_size);

/** Similar to `fj_realloc_uninit`, but all new items of the array are initialised to zeroes. */
fj_err fj_realloc_zeroed(void **ptr, size_t old_length, size_t new_length, size_t item_size);

/**
    The returned string must be freed manually.

    \returns the cloned string or NULL on failure.
*/
fj_err fj_alloc_string_copied(char const **out_clone, char const *source);


#endif
