#ifndef FEJIX_CORE_UTILS_H_
#define FEJIX_CORE_UTILS_H_


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
#define FJ_TRY(EXPR) for (fj_err_t fj_result = (EXPR); fj_result != FJ_OK; fj_result = FJ_OK)

/** Gets the length of a static array. Note: this double-evaluates the argument. */
#define FJ_LEN(ARRAY) (sizeof(ARRAY) / sizeof((ARRAY)[0]))

/** Note: this double-evaluates the arguments. */
#define FJ_MIN(A, B) (((A) < (B)) ? (A) : (B))

/** Note: this double-evaluates the arguments. */
#define FJ_MAX(A, B) (((A) > (B)) ? (A) : (B))

/** Note: this double-evaluates the arguments. */
#define FJ_CLAMP(X, MIN, MAX) FJ_MAX(FJ_MIN((X), (MAX)), (MIN))


/** */
FJ_EXPORT
fj_bool8_t fj_tag_eq(union fj_tag a, union fj_tag b, fj_tag_type_t type);

/**
    Do not hash pointers. When hashing pointers, this converts them to uintptr_t.
    Using the result is undefined behavior.
*/
FJ_EXPORT
uint32_t fj_tag_hash32(union fj_tag x, fj_tag_type_t type);

/** String can be NULLs as well. */
FJ_EXPORT
fj_bool8_t fj_str_eq(char const *a, char const *b);

/**
    The returned string must be freed manually.

    :param clone: Returns the cloned string or NULL on failure.
*/
FJ_EXPORT
fj_err_t fj_str_clone(char const *str, char const **clone);

/** If x >= 2^31, then returns UINT32_MAX */
FJ_EXPORT
uint32_t fj_u32_next_power_of_two(uint32_t x);

/** If x = 0, then returns 1. */
FJ_EXPORT
uint32_t fj_u32_prev_power_of_two(uint32_t x);

/**
    If x is greater than or equal to the highest power of two representable by size_t, then returns
    SIZE_MAX.
*/
FJ_EXPORT
size_t fj_size_next_power_of_two(size_t x);

/** If x = 0, then returns 1. */
FJ_EXPORT
size_t fj_size_prev_power_of_two(size_t x);

/** Returns the previous multiple of the divisor. */
FJ_EXPORT
uint32_t fj_u32_prev_multiple(uint32_t x, uint32_t divisor);

/** Returns the previous multiple of the divisor. */
FJ_EXPORT
uint64_t fj_u64_prev_multiple(uint64_t x, uint64_t divisor);

FJ_EXPORT
char const *fj_error_get_description(fj_err_t error);


#endif
