#ifndef FEJIX_UTILS_H_
#define FEJIX_UTILS_H_


#include <fejix/base.h>


#ifdef NDEBUG
#   define FJ_PROVIDE_VERSION(NAME, VERSION)
#   define FJ_REQUIRE_VERSION(NAME, VERSION)
#else
#   define FJ_PROVIDE_VERSION(NAME, VERSION) \
        typedef void FJ_VERSION_##VERSION##_##NAME;
#   define FJ_REQUIRE_VERSION(NAME, VERSION) \
        FJ_VERSION_##VERSION##_##NAME _fj_version_guard(void);
#endif

#ifdef NDEBUG
#   define FJ_UNUSED(X) (void) X;
#else
#   define FJ_UNUSED(X)
#endif

/* Get length of a static array. */
#define FJ_ARRLEN(ARRAY) (sizeof(ARRAY) / sizeof(*(ARRAY)))

#define FJ_STRINGIFY(ARG) FJ_STRINGIFY_IMPL(ARG)
#define FJ_STRINGIFY_IMPL(ARG) #ARG

#define FJ_TRY_INIT fj_err_t _fj_err = FJ_OK;
#define FJ_TRY _fj_err =
#define FJ_LAST_ERROR (_fj_err)
#define FJ_FAILED (_fj_err != FJ_OK)

/* `FJ_UTIL_FILENAME` is a short relative path to the current file.
    Ideally, CMake defines `FJ_FILENAME` for each Fejix source file.
    If this header gets compiled without CMake, it falls back to `__FILE__`. */
#ifdef FJ_FILENAME
#   define FJ_UTIL_FILENAME FJ_FILENAME
#else
#   define FJ_UTIL_FILENAME __FILE__
#endif

#define FJ_UTIL_LINE FJ_STRINGIFY(__LINE__)

#define FJ_UTIL_ERROR_TITLE "error [" FJ_UTIL_FILENAME ":" FJ_UTIL_LINE "]: "

/* Formats the given error message. `TEXT` must be a string literal. */
#define FJ_ERR(TEXT_LITERAL) FJ_STR(FJ_UTIL_ERROR_TITLE TEXT_LITERAL)

uint32_t fj_u32_max(uint32_t a, uint32_t b);

uint32_t fj_u32_min(uint32_t a, uint32_t b);

/* Accepts NULL as arguments. `NULL==NULL`, but `NULL!=""` */
fj_bool_t fj_str_eq(fj_string_t a, fj_string_t b);

#endif
