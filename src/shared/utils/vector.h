/**
    \file

    Example:

    \code{.c}

    FJ_VECTOR(my_vector, int)

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

#ifndef FEJIX_UTILS_VECTOR_H_
#define FEJIX_UTILS_VECTOR_H_


#include <fejix/base.h>


#define FJ_VECTOR_ARGS_(VECTOR) \
    ((void **) &vector->items), &vector->length, &vector->capacity, sizeof(*vector->items)

#define FJ_VECTOR(VECTOR_TYPE_NAME, ITEM_TYPE)                                          \
    struct VECTOR_TYPE_NAME {                                                           \
        ITEM_TYPE *items;                                                               \
        uint32_t length, capacity;                                                      \
    };                                                                                  \
                                                                                        \
    static inline enum fj_error VECTOR_TYPE_NAME##_insert(                              \
        struct VECTOR_TYPE_NAME *vector, uint32_t index, ITEM_TYPE *item)               \
    {                                                                                   \
        enum fj_error e = fj_vector_expand(FJ_VECTOR_ARGS_(vector), index);             \
        if (e)                                                                          \
            return e;                                                                   \
        vector->items[index] = *item;                                                   \
        return FJ_OK;                                                                   \
    }                                                                                   \
                                                                                        \
    static inline enum fj_error VECTOR_TYPE_NAME##_remove(                              \
        struct VECTOR_TYPE_NAME *vector, uint32_t index)                                \
    {                                                                                   \
        return fj_vector_shrink(FJ_VECTOR_ARGS_(vector), index);                        \
    }                                                                                   \
                                                                                        \
    static inline enum fj_error VECTOR_TYPE_NAME##_push(                                \
        struct VECTOR_TYPE_NAME *vector, ITEM_TYPE *item)                               \
    {                                                                                   \
        return VECTOR_TYPE_NAME##_insert(vector, vector->length, item);                 \
    }                                                                                   \
                                                                                        \
    static inline enum fj_error VECTOR_TYPE_NAME##_pop(struct VECTOR_TYPE_NAME *vector) \
    {                                                                                   \
        return VECTOR_TYPE_NAME##_remove(vector, vector->length - 1);                   \
    }                                                                                   \
                                                                                        \
    static inline void VECTOR_TYPE_NAME##_free(struct VECTOR_TYPE_NAME *vector)         \
    {                                                                                   \
        fj_vector_free(FJ_VECTOR_ARGS_(vector));                                        \
    }

/**
    The index must be between `0` and `length`.
    Returns `FJ_ERROR_INVALID_USAGE` or `FJ_ERROR_OUT_OF_MEMORY`.
*/
enum fj_error fj_vector_expand(
    void **items, uint32_t *length, uint32_t *capacity, size_t item_size, uint32_t index);

/**
    The index must be between `0` and `length-1`.
    Returns `FJ_ERROR_INVALID_USAGE` or `FJ_ERROR_OUT_OF_MEMORY`.
*/
enum fj_error fj_vector_shrink(
    void **items, uint32_t *length, uint32_t *capacity, size_t item_size, uint32_t index);

/**
    Frees the vector items and sets the length and capacity to zero.
*/
void fj_vector_free(void **items, uint32_t *length, uint32_t *capacity, size_t item_size);


#endif
