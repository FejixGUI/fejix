/**
    Contains convenient memory allocation functions and macros.

    Usage examples:

    .. code-block:: c

        int *x;

        FJ_ALLOC_UNINIT(&x);
        *x = 123;
        FJ_FREE(&x); // safely frees and nulls x
        assert(x == NULL);

        FJ_ALLOC_ZEROED(&x);
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
*///

#ifndef FEJIX_CORE_ALLOC_H_
#define FEJIX_CORE_ALLOC_H_


#include <fejix/core/base.h>


/** :see: :c:func:`fj_alloc_uninit` */
#define FJ_ALLOC_UNINIT(OBJECT_PTR_REF) \
    (fj_alloc_uninit((void *) (OBJECT_PTR_REF), sizeof(**(OBJECT_PTR_REF))))

/** :see: :c:func:`fj_alloc_zeroed` */
#define FJ_ALLOC_ZEROED(OBJECT_PTR_REF) \
    (fj_alloc_zeroed((void *) (OBJECT_PTR_REF), sizeof(**(OBJECT_PTR_REF))))

/** :see: :c:func:`fj_alloc_copied` */
#define FJ_ALLOC_COPIED(OBJECT_PTR_REF, SOURCE) \
    (fj_alloc_copied((void *) (OBJECT_PTR_REF), (void *) (SOURCE), sizeof(**(OBJECT_PTR_REF))))

/** :see: :c:func:`fj_realloc_uninit` */
#define FJ_REALLOC_UNINIT(ARRAY_PTR_REF, NEW_COUNT) \
    (fj_realloc_uninit((void *) (ARRAY_PTR_REF), (uint32_t) NEW_COUNT, sizeof(**(ARRAY_PTR_REF))))

/** :see: :c:func:`fj_realloc_zeroed` */
#define FJ_REALLOC_ZEROED(ARRAY_PTR_REF, OLD_COUNT, NEW_COUNT) \
    (fj_realloc_zeroed(                                        \
        (void *) (ARRAY_PTR_REF),                              \
        (uint32_t) (OLD_COUNT),                                \
        (uint32_t) (NEW_COUNT),                                \
        sizeof(**(ARRAY_PTR_REF))                              \
    ))

/** :see: :c:func:`fj_free` */
#define FJ_FREE(OBJECT_PTR_REF) (fj_free((void *) (OBJECT_PTR_REF)))


FJ_EXTERN_C_BEGIN

/**
    Allocates an uninitialized block of memory.
    Works like ``malloc``, but allocating 0 bytes is always an error.

    :param ptr: Returns a new pointer or NULL on failure.
*/
fj_err_t fj_alloc_uninit(void **ptr, size_t size);

/**
    Allocates a block of memory initialized with zeros.
    Works like ``calloc(1,)``, but allocating 0 bytes is always an error.

    :param ptr: Returns a new pointer or NULL on failure.
*/
fj_err_t fj_alloc_zeroed(void **ptr, size_t size);

/**
    Allocates a block of memory initialised with bytes from ``source``.

    :param ptr: Returns a new pointer or NULL on failure.
*/
fj_err_t fj_alloc_copied(void **ptr, void const *source, size_t size);

/**
    Frees a block of memory.

    :param ptr: References the pointer to be freed, always returns NULL.
*/
void fj_free(void **ptr);

/**
    Reallocates a block of memory for an array of items.

    If reallocation fails, this does not change the given pointer and does not free the old block.

    * ``fj_realloc_uninit(NULL, 0, y) = nothing``
    * ``fj_realloc_uninit(NULL, x, y) = alloc_uninit(x*y)``
    * ``fj_realloc_uninit(ptr,  0, y) = free(ptr)``
    * ``fj_realloc_uninit(ptr,  x, y) = realloc_uninit(ptr, x*y)``

    :param ptr: Returns a new pointer on success, does not change the pointer on failure.
*/
fj_err_t fj_realloc_uninit(void **ptr, uint32_t item_count, size_t item_size);

/**
    Similar to :c:func:`fj_realloc_uninit`, but all new items of the array are initialised to
    zeroes.
*/
fj_err_t fj_realloc_zeroed(
    void **ptr,
    uint32_t old_item_count,
    uint32_t new_item_count,
    size_t item_size
);


FJ_EXTERN_C_END

#endif
