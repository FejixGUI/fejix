/** Example:

    ```
    FJ_DEFINE_VECTOR(my_vector, int)

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
    ```
*/

#ifndef FEJIX_UTILS_VECTOR_H_INCLUDED
#define FEJIX_UTILS_VECTOR_H_INCLUDED


#include <fejix/core.h>


#define FJ_DEFINE_VECTOR(VECTOR_TYPE_NAME, ITEM_TYPE) \
    struct VECTOR_TYPE_NAME {                         \
        ITEM_TYPE *items;                             \
        uint32_t length, capacity;                    \
    };

#define FJ_VECTOR_EXPAND_AT(VECTOR, INDEX) \
    (fj_vector_expand_at(                  \
        (void **) &(VECTOR)->items,        \
        &(VECTOR)->length,                 \
        &(VECTOR)->capacity,               \
        (INDEX),                           \
        sizeof(*(VECTOR)->items)))

#define FJ_VECTOR_EXPAND(VECTOR)    \
    (fj_vector_expand_at(           \
        (void **) &(VECTOR)->items, \
        &(VECTOR)->length,          \
        &(VECTOR)->capacity,        \
        (VECTOR)->length,           \
        sizeof(*(VECTOR)->items)))

#define FJ_VECTOR_SHRINK_AT(VECTOR, INDEX) \
    (fj_vector_shrink_at(                  \
        (void **) &(VECTOR)->items,        \
        &(VECTOR)->length,                 \
        &(VECTOR)->capacity,               \
        (INDEX),                           \
        sizeof(*(VECTOR)->items)))

#define FJ_VECTOR_SHRINK(VECTOR)                             \
    ((VECTOR)->length == 0 ? FJ_ERR_INVALID_USAGE            \
                           : fj_vector_shrink_at(            \
                                 (void **) &(VECTOR)->items, \
                                 &(VECTOR)->length,          \
                                 &(VECTOR)->capacity,        \
                                 (VECTOR)->length - 1,       \
                                 sizeof(*(VECTOR)->items)))

#define FJ_VECTOR_FREE(VECTOR) \
    (fj_vector_free((void **) &(VECTOR)->items, &(VECTOR)->length, &(VECTOR)->capacity))

/** The index must be between `0` and `length`.
    Returns `FJ_ERR_INVALID_USAGE` or `FJ_ERR_OUT_OF_MEMORY`. */
FJ_PUBLIC
fj_err fj_vector_expand_at(
    void **items, uint32_t *length, uint32_t *capacity, uint32_t index, size_t item_size);

/** The index must be between `0` and `length-1`.
    Returns `FJ_ERR_INVALID_USAGE` or `FJ_ERR_OUT_OF_MEMORY`. */
FJ_PUBLIC
fj_err fj_vector_shrink_at(
    void **items, uint32_t *length, uint32_t *capacity, uint32_t index, size_t item_size);

/** Frees the vector items and sets the length and capacity to
 * zero. */
FJ_PUBLIC
void fj_vector_free(void **items, uint32_t *length, uint32_t *capacity);


#endif
