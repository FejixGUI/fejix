#ifndef FEJIX_UTILS_H_
#define FEJIX_UTILS_H_


#include <fejix/base.h>


/* Idea of a per-object static version check.

#ifdef NDEBUG
#   define FJ_PROVIDE_VERSION(NAME, VERSION)
#   define FJ_REQUIRE_VERSION(NAME, VERSION)
#else
#   define FJ_PROVIDE_VERSION(NAME, VERSION) \
        typedef void (FJ_VERSION_##VERSION##_##NAME)(void);
#   define FJ_REQUIRE_VERSION(NAME, VERSION) \
        FJ_VERSION_##VERSION##_##NAME _fj_version_guard;
#endif
*/

#ifdef NDEBUG
#   define FJ_UNUSED(X) (void) X;
#else
#   define FJ_UNUSED(X)
#endif

/** Get length of a fixed-length array. */
#define FJ_ARRLEN(ARRAY) (sizeof(ARRAY) / sizeof((ARRAY)[0]))

#define FJ_STRINGIFY(ARG) FJ_STRINGIFY_IMPL(ARG)
#define FJ_STRINGIFY_IMPL(ARG) #ARG

#define FJ_INIT_ERRORS fj_err_t _fj_err = FJ_OK; FJ_UNUSED(_fj_err);
#define FJ_TRY _fj_err =
#define FJ_LAST_ERROR (_fj_err)
#define FJ_FAILED (_fj_err != FJ_OK)

/** `FJ_UTIL_FILENAME` is a short relative path to the current file.
    Ideally, our build system defines `FJ_FILENAME` for each source file.
    If this header gets compiled without the build system, this macro
    falls back to `__FILE__`. */
#ifdef FJ_FILENAME
#   define FJ_UTIL_FILENAME FJ_FILENAME
#else
#   define FJ_UTIL_FILENAME __FILE__
#endif

#define FJ_UTIL_LINE FJ_STRINGIFY(__LINE__)

#define FJ_UTIL_ERROR_HEADER "[" FJ_UTIL_FILENAME ":" FJ_UTIL_LINE "] "

/** Formats the given error message. */
#define FJ_ERR(TEXT_LITERAL) FJ_UTF8(FJ_UTIL_ERROR_HEADER TEXT_LITERAL)


/** Double-evaluates the arguments. */
#define FJ_MIN(A, B) (((A) < (B)) ? (A) : (B))

/** Double-evaluates the arguments. */
#define FJ_MAX(A, B) (((A) > (B)) ? (A) : (B))


uint32_t fj_uint32_hash32(uint32_t x);

uint32_t fj_uint64_hash32(uint64_t x);

/** Requires that pointers support clear conversions to `uintptr_t`, that is,
    if `a_ptr == b_ptr`, then `(uintptr_t) a_ptr == (uintptr_t) b_ptr`,
    which is in fact not guaranteed by the C99 standard, however works on most
    target platforms. */
uint32_t fj_uintptr_hash32(uintptr_t x);

/** Accepts NULL as arguments. `NULL==NULL`, but `NULL!=""` */
fj_bool_t fj_str_eq(fj_string_t FJ_NULLABLE a, fj_string_t FJ_NULLABLE b);

#endif
