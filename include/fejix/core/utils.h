#ifndef FEJIX_CORE_UTILS_H_
#define FEJIX_CORE_UTILS_H_


#include <fejix/core/error.h>

#include <string.h>


#define FJ_STRINGIFY(X) FJ_STRINGIFY_IMPL(X)
#define FJ_STRINGIFY_IMPL(X) #X

#define FJ_FILEPOS __FILE__ ":" FJ_STRINGIFY(__LINE__)

#define FJ_UNUSED(VALUE) (void) VALUE;

/** Get length of a fixed-length array. */
#define FJ_ARRAY_LEN(ARRAY) (sizeof(ARRAY) / sizeof((ARRAY)[0]))

/** Double-evaluates the arguments. */
#define FJ_MIN(A, B) (((A) < (B)) ? (A) : (B))

/** Double-evaluates the arguments. */
#define FJ_MAX(A, B) (((A) > (B)) ? (A) : (B))

/** Double-evaluates the arguments. */
#define FJ_CLAMP(X, MIN, MAX) FJ_MAX(FJ_MIN((X), (MAX)), (MIN))

/** Does not check if the arguments are NULL. */
#define FJ_STRING_EQ(A, B) (strcmp((char const *)(A), (char const *)(B)) == 0)


#endif
