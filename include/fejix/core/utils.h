#ifndef FEJIX_CORE_UTILS_H_
#define FEJIX_CORE_UTILS_H_


#include <fejix/core/base.h>


#define FJ_TRY(EXPR)                                   \
    for (fj_err_t fj_result = (EXPR), i = 1; i; i = 0) \
        if (fj_result != FJ_OK)

#define FJ_STRINGIFY(X) FJ_STRINGIFY_IMPL(X)
#define FJ_STRINGIFY_IMPL(X) #X

#define FJ_FILEPOS __FILE__ ":" FJ_STRINGIFY(__LINE__)

/** Get length of a fixed-length array. */
#define FJ_ARRAY_LEN(ARRAY) (sizeof(ARRAY) / sizeof((ARRAY)[0]))

/** Double-evaluates the arguments. */
#define FJ_MIN(A, B) (((A) < (B)) ? (A) : (B))

/** Double-evaluates the arguments. */
#define FJ_MAX(A, B) (((A) > (B)) ? (A) : (B))

/** Double-evaluates the arguments. */
#define FJ_CLAMP(X, MIN, MAX) FJ_MAX(FJ_MIN((X), (MAX)), (MIN))


FJ_EXTERN_C_BEGIN


fj_bool8_t fj_str_eq(char const *a, char const *b);

/**
The returned string must be freed manually.

:param clone: Returns the cloned string or NULL on failure.
*/
fj_err_t fj_str_clone(char const *str, char const **clone);

/** If x >= 2^31, then returns UINT32_MAX */
uint32_t fj_u32_next_power_of_two(uint32_t x);

/** If x = 0, then returns 1. */
uint32_t fj_u32_prev_power_of_two(uint32_t x);

/**
If x is greater than or equal to the highest power of two representable by size_t, then returns
SIZE_MAX.
*/
size_t fj_size_next_power_of_two(size_t x);

/** If x = 0, then returns 1. */
size_t fj_size_prev_power_of_two(size_t x);

/** Returns the previous multiple of the divisor. */
uint32_t fj_u32_prev_multiple(uint32_t x, uint32_t divisor);

/** Returns the previous multiple of the divisor. */
uint64_t fj_u64_prev_multiple(uint64_t x, uint64_t divisor);


FJ_EXTERN_C_BEGIN

#endif
