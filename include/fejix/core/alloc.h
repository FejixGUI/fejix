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
#define FJ_ALLOC_UNINIT(OUT_OBJECT_PTR) \
    (fj_alloc_uninit((void **) (OUT_OBJECT_PTR), sizeof(**(OUT_OBJECT_PTR))))

/** :see: :c:func:`fj_alloc_zeroed` */
#define FJ_ALLOC_ZEROED(OUT_OBJECT_PTR) \
    (fj_alloc_zeroed((void **) (OUT_OBJECT_PTR), sizeof(**(OUT_OBJECT_PTR))))

/** :see: :c:func:`fj_alloc_copied` */
#define FJ_ALLOC_COPIED(OUT_OBJECT_PTR, SOURCE) \
    (fj_alloc_copied((void **) (OUT_OBJECT_PTR), (void *) (SOURCE), sizeof(**(OUT_OBJECT_PTR))))

/** :see: :c:func:`fj_realloc_uninit` */
#define FJ_REALLOC_UNINIT(OUT_ARRAY_PTR, NEW_COUNT) \
    (fj_realloc_uninit((void **) (OUT_ARRAY_PTR), (uint32_t) NEW_COUNT, sizeof(**(OUT_ARRAY_PTR))))

/** :see: :c:func:`fj_realloc_zeroed` */
#define FJ_REALLOC_ZEROED(OUT_ARRAY_PTR, OLD_COUNT, NEW_COUNT) \
    (fj_realloc_zeroed(                                        \
        (void **) (OUT_ARRAY_PTR),                             \
        (uint32_t) (OLD_COUNT),                                \
        (uint32_t) (NEW_COUNT),                                \
        sizeof(**(OUT_ARRAY_PTR))))

/** :see: :c:func:`fj_free` */
#define FJ_FREE(OUT_ARRAY_PTR) (fj_free((void **) (OUT_ARRAY_PTR)))


/** Custom allocation callbacks. */
struct fj_alloc_callbacks {
    /** Same as the standard malloc. */
    void *(*malloc)(size_t size);

    /** Same as the standard calloc. */
    void *(*calloc)(size_t count, size_t size);

    /** Same as the standard realloc. */
    void *(*realloc)(void *ptr, size_t size);

    /** Same as the standard free. */
    void (*free)(void *ptr);
};


/**
    Allocates an uninitialized block of memory.
    Works like ``malloc``, but allocating 0 bytes is always an error.

    :param out_ptr: Returns a new pointer or NULL on failure.
*/
FJ_PUBLIC
fj_err_t fj_alloc_uninit(void **out_ptr, size_t size);

/**
    Allocates a block of memory initialized with zeros.
    Works like ``calloc(1,)``, but allocating 0 bytes is always an error.

    :param out_ptr: Returns a new pointer or NULL on failure.
*/
FJ_PUBLIC
fj_err_t fj_alloc_zeroed(void **out_ptr, size_t size);

/**
    Allocates a block of memory initialised with bytes from ``source``.

    :param out_ptr: Returns a new pointer or NULL on failure.
*/
FJ_PUBLIC
fj_err_t fj_alloc_copied(void **out_ptr, void const *source, size_t size);

/**
    Frees a block of memory.

    :param out_ptr: References the pointer to be freed, always returns NULL.
*/
FJ_PUBLIC
void fj_free(void **out_ptr);

/**
    Reallocates a block of memory for an array of items.

    If reallocation fails, this does not change the given pointer and does not free the old block.

    * ``fj_realloc_uninit(NULL, 0, y) = nothing``
    * ``fj_realloc_uninit(NULL, x, y) = alloc_uninit(x*y)``
    * ``fj_realloc_uninit(out_ptr,  0, y) = free(out_ptr)``
    * ``fj_realloc_uninit(out_ptr,  x, y) = realloc_uninit(out_ptr, x*y)``

    :param out_ptr: Returns a new pointer on success, does not change the pointer on failure.
*/
FJ_PUBLIC
fj_err_t fj_realloc_uninit(void **out_ptr, uint32_t item_count, size_t item_size);

/**
    Similar to :c:func:`fj_realloc_uninit`, but all new items of the array are initialised to
    zeroes.
*/
FJ_PUBLIC
fj_err_t fj_realloc_zeroed(
    void **out_ptr, uint32_t old_item_count, uint32_t new_item_count, size_t item_size);


/** :param callbacks: If NULL, the callbacks will be reset to the defaults. */
FJ_PUBLIC
void fj_alloc_set_callbacks(struct fj_alloc_callbacks const *callbacks);

/** If no custom callbacks have been set, the default standard allocation functions are returned. */
FJ_PUBLIC
struct fj_alloc_callbacks const *fj_alloc_get_callbacks(void);


#endif
