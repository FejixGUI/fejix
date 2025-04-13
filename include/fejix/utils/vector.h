/**
    \file

    Example:

    \code{.c}

    FJ_VECTOR_OF(my_vector, int)

    // Vectors must be initialised with zeroes!
    struct my_vector v = { 0 };

    int a = 111, b = 222, c = 333, d = 444;

    my_vector_push(&v, &a);

    my_vector_push(&v, &b);

    my_vector_push(&v, &d);

    my_vector_insert(&v, 2, &c);

    // Prints: 111 222 333 444
    for (uint32_t i=0; i<v.length; i++) {
        printf("%d ", v.items[i]);
    }

    my_vector_remove(&v, 2);

    // Prints: 111 222 444
    for (uint32_t i=0; i<v.length; i++) {
        printf("%d ", v.items[i]);
    }

    my_vector_free(&v);

    \endcode
*/

#ifndef FEJIX_UTILS_VECTOR_H_INCLUDED
#define FEJIX_UTILS_VECTOR_H_INCLUDED


#include <fejix/core.h>


#define FJ_VECTOR_OF(VECTOR_TYPE_NAME, ITEM_TYPE)                                                  \
    struct VECTOR_TYPE_NAME {                                                                      \
        ITEM_TYPE *items;                                                                          \
        uint32_t length, capacity;                                                                 \
    };                                                                                             \
                                                                                                   \
    static inline fj_err VECTOR_TYPE_NAME##_insert(                                                \
        struct VECTOR_TYPE_NAME *vector, uint32_t index, ITEM_TYPE *item)                          \
    {                                                                                              \
        FJ_TRY (fj_vector_expand_at(                                                               \
                    (void **) &vector->items,                                                      \
                    &vector->length,                                                               \
                    &vector->capacity,                                                             \
                    index,                                                                         \
                    sizeof(*vector->items)))                                                       \
        {                                                                                          \
            return fj_result;                                                                      \
        }                                                                                          \
                                                                                                   \
        vector->items[index] = *item;                                                              \
        return FJ_OK;                                                                              \
    }                                                                                              \
                                                                                                   \
    static inline fj_err VECTOR_TYPE_NAME##_remove(                                                \
        struct VECTOR_TYPE_NAME *vector, uint32_t index)                                           \
    {                                                                                              \
        return fj_vector_shrink_at(                                                                \
            (void **) &vector->items,                                                              \
            &vector->length,                                                                       \
            &vector->capacity,                                                                     \
            index,                                                                                 \
            sizeof(*vector->items));                                                               \
    }                                                                                              \
                                                                                                   \
    static inline fj_err VECTOR_TYPE_NAME##_push(struct VECTOR_TYPE_NAME *vector, ITEM_TYPE *item) \
    {                                                                                              \
        return VECTOR_TYPE_NAME##_insert(vector, vector->length, item);                            \
    }                                                                                              \
                                                                                                   \
    static inline fj_err VECTOR_TYPE_NAME##_pop(struct VECTOR_TYPE_NAME *vector)                   \
    {                                                                                              \
        if (vector->length == 0) {                                                                 \
            return FJ_ERR_INVALID_USAGE;                                                           \
        }                                                                                          \
                                                                                                   \
        return VECTOR_TYPE_NAME##_remove(vector, vector->length - 1);                              \
    }                                                                                              \
                                                                                                   \
    static inline void VECTOR_TYPE_NAME##_free(struct VECTOR_TYPE_NAME *vector)                    \
    {                                                                                              \
        fj_vector_free((void **) &vector->items, &vector->length, &vector->capacity);              \
    }

/**
    The index must be between `0` and `length`.
    Returns `FJ_ERR_INVALID_USAGE` or `FJ_ERR_OUT_OF_MEMORY`.
*/
FJ_PUBLIC
fj_err fj_vector_expand_at(
    void **items, uint32_t *length, uint32_t *capacity, uint32_t index, size_t item_size);

/**
    The index must be between `0` and `length-1`.
    Returns `FJ_ERR_INVALID_USAGE` or `FJ_ERR_OUT_OF_MEMORY`.
*/
FJ_PUBLIC
fj_err fj_vector_shrink_at(
    void **items, uint32_t *length, uint32_t *capacity, uint32_t index, size_t item_size);

/**
    Frees the vector items and sets the length and capacity to zero.
*/
FJ_PUBLIC
void fj_vector_free(void **items, uint32_t *length, uint32_t *capacity);


#endif
