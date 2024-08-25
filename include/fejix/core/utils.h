#ifndef FEJIX_CORE_UTILS_H_
#define FEJIX_CORE_UTILS_H_


#include <fejix/core/base.h>


#define FJ_INIT_TRY fj_err_t _fj_err = FJ_OK;
#define FJ_TRY(EXPR) _fj_err = (EXPR); if (_fj_err != FJ_OK)
#define FJ_CATCH(ERROR) if (_fj_err == (ERROR))
#define FJ_RESULT (_fj_err)

#define FJ_STRINGIFY(X) FJ_STRINGIFY_IMPL(X)
#define FJ_STRINGIFY_IMPL(X) #X

#define FJ_FILEPOS __FILE__ ":" FJ_STRINGIFY(__LINE__)

#define FJ_ARG_UNUSED(ARG) (void) _##ARG;

#define FJ_ARG_FROM_OPAQUE(ARG, TRANSPARENT_TYPE) TRANSPARENT_TYPE ARG = (void *) ARG##_;

/** Get length of a fixed-length array. */
#define FJ_ARRAY_LEN(ARRAY) (sizeof(ARRAY) / sizeof((ARRAY)[0]))

/** Double-evaluates the arguments. */
#define FJ_MIN(A, B) (((A) < (B)) ? (A) : (B))

/** Double-evaluates the arguments. */
#define FJ_MAX(A, B) (((A) > (B)) ? (A) : (B))

/** Double-evaluates the arguments. */
#define FJ_CLAMP(X, MIN, MAX) FJ_MAX(FJ_MIN((X), (MAX)), (MIN))


fj_bool32_t fj_streq(char const */*[]*/ a, char const */*[]*/ b);

/** The returned string must be freed manually. */
fj_err_t fj_strdup(char const */*[]*/ str, char const */*[]? out*/ * clone);

/** If x >= 2^31, then returns UINT32_MAX */
uint32_t fj_u32_next_power_of_two(uint32_t x);

/** If x = 0, then returns 1. */
uint32_t fj_u32_prev_power_of_two(uint32_t x);

/** If x is greater or equal to the highest power of two representable by size_t,
    then returns SIZE_MAX */
size_t fj_size_next_power_of_two(size_t x);

/** If x = 0, then returns 1. */
size_t fj_size_prev_power_of_two(size_t x);

fj_bool32_t fj_dummy_true(void);

fj_bool32_t fj_dummy_false(void);

fj_err_t fj_dummy_ok(void);

void fj_dummy_void(void);


#endif
