/**
    Example:

    \code{.c}

    FJ_LIST(my_list, int)

    // Lists must be initialised with zeroes!
    struct my_list l = { 0 };

    int a = 111, b = 222, c = 333, d = 444;

    my_list_append(&v, &a);

    my_list_append(&v, &b);

    my_list_append(&v, &d);

    my_list_insert(&v, 2, &c);

    // Prints: 111 222 333 444
    for (uint32_t i=0; i<v.length; i++) {
        printf("%d ", v.items[i]);
    }

    my_list_remove(&v, 2);

    // Prints: 111 222 444
    for (uint32_t i=0; i<v.length; i++) {
        printf("%d ", v.items[i]);
    }

    my_list_deinit(&v);

    \endcode
*/

#ifndef FEJIX_UTILS_LIST_H_
#define FEJIX_UTILS_LIST_H_


#include <fejix/base.h>


#define FJ_LIST_ARGS_(LIST) \
    ((void **) &list->items), &list->length, &list->capacity, sizeof(*list->items)

#define FJ_LIST(LIST_TYPE_NAME, ITEM_TYPE)                                         \
    struct LIST_TYPE_NAME {                                                        \
        ITEM_TYPE *items;                                                          \
        uint32_t length, capacity;                                                 \
    };                                                                             \
                                                                                   \
    static inline enum fj_status LIST_TYPE_NAME##_insert(                          \
        struct LIST_TYPE_NAME *list, uint32_t index, ITEM_TYPE *item)              \
    {                                                                              \
        enum fj_status s = fj_list_expand(FJ_LIST_ARGS_(list), index);             \
        if (s)                                                                     \
            return s;                                                              \
        list->items[index] = *item;                                                \
        return FJ_OK;                                                              \
    }                                                                              \
                                                                                   \
    static inline enum fj_status LIST_TYPE_NAME##_remove(                          \
        struct LIST_TYPE_NAME *list, uint32_t index)                               \
    {                                                                              \
        return fj_list_shrink(FJ_LIST_ARGS_(list), index);                         \
    }                                                                              \
                                                                                   \
    static inline enum fj_status LIST_TYPE_NAME##_append(                          \
        struct LIST_TYPE_NAME *list, ITEM_TYPE *item)                              \
    {                                                                              \
        return LIST_TYPE_NAME##_insert(list, list->length, item);                  \
    }                                                                              \
                                                                                   \
    static inline enum fj_status LIST_TYPE_NAME##_pop(struct LIST_TYPE_NAME *list) \
    {                                                                              \
        return LIST_TYPE_NAME##_remove(list, list->length - 1);                    \
    }                                                                              \
                                                                                   \
    static inline void LIST_TYPE_NAME##_free(struct LIST_TYPE_NAME *list)          \
    {                                                                              \
        fj_list_free(FJ_LIST_ARGS_(list));                                         \
    }

/**
    The index must be between `0` and `length`.
    Returns `FJ_ERROR_INVALID_USAGE` or `FJ_ERROR_OUT_OF_MEMORY`.
*/
enum fj_status fj_list_expand(
    void **items, uint32_t *length, uint32_t *capacity, size_t item_size, uint32_t index);

/**
    The index must be between `0` and `length-1`.
    Returns `FJ_ERROR_INVALID_USAGE` or `FJ_ERROR_OUT_OF_MEMORY`.
*/
enum fj_status fj_list_shrink(
    void **items, uint32_t *length, uint32_t *capacity, size_t item_size, uint32_t index);

/**
    Frees the list items and sets the length and capacity to zero.
*/
void fj_list_free(void **items, uint32_t *length, uint32_t *capacity, size_t item_size);


#endif
