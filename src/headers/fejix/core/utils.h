#ifndef FEJIX_CORE_UTILS_INCLUDED
#define FEJIX_CORE_UTILS_INCLUDED


#include <fejix/core/base.h>


/**
    If the expression returns an error, executes the block next to it.

    Syntax:

    .. code-block:: c

        FJ_TRY (<expression>) {
            <code>
            // the result of the expression is stored in fj_result variable
        }

    Example:

    .. code-block:: c

        FJ_TRY (FJ_ERR_ALLOCATION_FAILED) {
            assert(fj_result == FJ_ERR_ALLOCATION_FAILED);
        }
*/
#define FJ_TRY(EXPR)                                                                            \
    for (fj_err_t fj_result = (EXPR), _fj_try_guard = 1; _fj_try_guard == 1; _fj_try_guard = 0) \
        if (fj_result != FJ_OK)  // Outside of for loop to avoid dead code if the block returns

/** Gets the length of a static array. Note: this double-evaluates the argument. */
#define FJ_LEN(ARRAY) (sizeof(ARRAY) / sizeof((ARRAY)[0]))


/**
    The returned string must be freed manually.

    :param clone: Returns the cloned string or NULL on failure.
*/
FJ_PUBLIC
fj_err_t fj_string_clone(char const *str, char const **out_clone);

/** If x >= 2^31, then returns UINT32_MAX */
FJ_PUBLIC
uint32_t fj_u32_next_power_of_two(uint32_t x);

/** If x = 0, then returns 1. */
FJ_PUBLIC
uint32_t fj_u32_prev_power_of_two(uint32_t x);

/**
    If x is greater than or equal to the highest power of two representable by size_t, then returns
    SIZE_MAX.
*/
FJ_PUBLIC
size_t fj_size_next_power_of_two(size_t x);

/** If x = 0, then returns 1. */
FJ_PUBLIC
size_t fj_size_prev_power_of_two(size_t x);

/** Returns the previous multiple of the divisor. */
FJ_PUBLIC
uint32_t fj_u32_prev_multiple(uint32_t x, uint32_t divisor);

/** Returns the previous multiple of the divisor. */
FJ_PUBLIC
uint64_t fj_u64_prev_multiple(uint64_t x, uint64_t divisor);

static inline uint32_t fj_u32_max(uint32_t a, uint32_t b)
{
    return a > b ? a : b;
}

static inline uint32_t fj_u32_min(uint32_t a, uint32_t b)
{
    return a < b ? a : b;
}


#endif
