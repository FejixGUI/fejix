#ifndef FEJIX_CORE_UTILS_H_
#define FEJIX_CORE_UTILS_H_


#include "fejix/core/base.h"
#include <fejix/core/error.h>



#define FJ_STRINGIFY(X) FJ_STRINGIFY_IMPL(X)
#define FJ_STRINGIFY_IMPL(X) #X

#define FJ_FILEPOS __FILE__ ":" FJ_STRINGIFY(__LINE__)

#define FJ_ARG_UNUSED(ARG) (void) ARG;

#define FJ_ARG_FROM_OPAQUE(OPAQUE_ARG, VAR_DECL) VAR_DECL = (void *) OPAQUE_ARG;

#define FJ_INTO_BASE_PTR(OBJECT_PTR) (&((OBJECT_PTR)->base))

/** Get length of a fixed-length array. */
#define FJ_ARRAY_LEN(ARRAY) (sizeof(ARRAY) / sizeof((ARRAY)[0]))

/** Double-evaluates the arguments. */
#define FJ_MIN(A, B) (((A) < (B)) ? (A) : (B))

/** Double-evaluates the arguments. */
#define FJ_MAX(A, B) (((A) > (B)) ? (A) : (B))

/** Double-evaluates the arguments. */
#define FJ_CLAMP(X, MIN, MAX) FJ_MAX(FJ_MIN((X), (MAX)), (MIN))


fj_bool32_t fj_streq(uint8_t const */*[]*/ a, uint8_t const */*[]*/ b);

/** The returned string must be freed manually. */
fj_err_t fj_strdup(uint8_t const */*[]*/ str, uint8_t const */*[]? out*/ * clone);


#endif
