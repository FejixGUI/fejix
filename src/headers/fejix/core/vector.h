/**
    Example:

    .. code-block:: c

        struct my_vector {
            FJ_VECTOR(int)
        };

        // Vectors must be initialised with zeroes!
        struct my_vector v = { 0 };

        FJ_VECTOR_EXPAND(&v);
        v.items[0] = 111;

        FJ_VECTOR_EXPAND(&v);
        v.items[1] = 222;

        FJ_VECTOR_EXPAND(&v);
        v.items[2] = 444;

        FJ_VECTOR_EXPAND_AT(&v, 2);
        v.items[2] = 333;

        // Prints: 111 222 333 444
        for (uint32_t i=0; i<v.length; i++) {
            printf("%d ", v.items[i]);
        }

        FJ_VECTOR_SHRINK_AT(&v, 2);

        // Prints: 111 222 444
        for (uint32_t i=0; i<v.length; i++) {
            printf("%d ", v.items[i]);
        }

        FJ_VECTOR_FREE(&v);

*///

#ifndef FEJIX_CORE_VECTOR_INCLUDED
#define FEJIX_CORE_VECTOR_INCLUDED


#include <fejix/core/base.h>


/** Defines the fields of a vector structure. */
#define FJ_VECTOR(ITEM_TYPE) \
    ITEM_TYPE *items;        \
    uint32_t length, capacity;

/** */
#define FJ_VECTOR_EXPAND_AT(VECTOR, INDEX) \
    (fj_vector_expand_at(                  \
        (void **) &(VECTOR)->items,        \
        &(VECTOR)->length,                 \
        &(VECTOR)->capacity,               \
        (INDEX),                           \
        sizeof(*(VECTOR)->items)))

/** */
#define FJ_VECTOR_SHRINK_AT(VECTOR, INDEX) \
    (fj_vector_shrink_at(                  \
        (void **) &(VECTOR)->items,        \
        &(VECTOR)->length,                 \
        &(VECTOR)->capacity,               \
        (INDEX),                           \
        sizeof(*(VECTOR)->items)))

/** */
#define FJ_VECTOR_EXPAND(VECTOR)    \
    (fj_vector_expand(              \
        (void **) &(VECTOR)->items, \
        &(VECTOR)->length,          \
        &(VECTOR)->capacity,        \
        sizeof(*(VECTOR)->items)))

/** */
#define FJ_VECTOR_SHRINK(VECTOR)    \
    (fj_vector_shrink(              \
        (void **) &(VECTOR)->items, \
        &(VECTOR)->length,          \
        &(VECTOR)->capacity,        \
        sizeof(*(VECTOR)->items)))

/** */
#define FJ_VECTOR_FREE(VECTOR) \
    (fj_vector_free((void **) &(VECTOR)->items, &(VECTOR)->length, &(VECTOR)->capacity))

/**
    :param index: The index between ``0`` and ``length-1``.
    :returns: ``FJ_ERR_INVALID_INDEX``, ``FJ_ERR_CANNOT_ALLOCATE`` etc.
*/
FJ_PUBLIC
fj_err_t fj_vector_expand_at(
    void **items, uint32_t *length, uint32_t *capacity, uint32_t index, size_t item_size);

/**
    :param index: The index between ``0`` and ``length-1``.
    :returns: ``FJ_ERR_INVALID_INDEX``, ``FJ_ERR_CANNOT_ALLOCATE`` etc.
*/
FJ_PUBLIC
fj_err_t fj_vector_shrink_at(
    void **items, uint32_t *length, uint32_t *capacity, uint32_t index, size_t item_size);

FJ_PUBLIC
fj_err_t fj_vector_expand(void **items, uint32_t *length, uint32_t *capacity, size_t item_size);

/**
    Removes the last item of the vector.
    :returns: ``FJ_ERR_VECTOR_EMPTY`` if cannot remove any items or other errors.
*/
FJ_PUBLIC
fj_err_t fj_vector_shrink(void **items, uint32_t *length, uint32_t *capacity, size_t item_size);

/** Frees the vector items and sets the length and capacity to zero. */
FJ_PUBLIC
void fj_vector_free(void **items, uint32_t *length, uint32_t *capacity);


#endif
