#ifndef FEJIX_COMMON_MACROS_H_
#define FEJIX_COMMON_MACROS_H_

/** Gets the length of a static array. */
#define FJ_LEN(ARRAY) (sizeof(ARRAY) / sizeof((ARRAY)[0]))

/** Example usage:
    \code{.c}

    // In some header

    #define MY_MACROLIST FJ_MACROLIST(FJ_MACROITEM(X) FJ_MACROITEM(Y))

    // In some file

    #undef FJ_MACROLIST
    #undef FJ_MACROITEM
    #define FJ_MACROLIST(...) enum my_enum { __VA_ARGS__ MY_ENUM_MAX };
    #define FJ_MACROITEM(ITEM) MY_##ITEM,
    MY_MACROLIST

    // ...which expands to...

    enum my_enum { MY_X, MY_Y, MY_ENUM_MAX };

    \endcode

    These macros here are defined only for the sake of this comment.
    They should be #undef'ed and #define'd every time macrolists are used. */
#define FJ_MACROLIST(...)
#define FJ_MACROITEM(ITEM)

#endif
