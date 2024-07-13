#ifndef FEJIX_INTERNAL_WINAPI_UTILS_H_
#define FEJIX_INTERNAL_WINAPI_UTILS_H_


#include "fejix/base.h"
#include <windows.h>


/** Converts UTF-8 to Windows multibyte ("wide") encoding.
    The returned string must be freed with `fj_free`.

    === Returns ===

    Returns an error if the given string is invalid UTF-8. */
fj_err_t fj_winapi_utf8_to_wstr(
    fj_string_t utf8_string,
    LPWSTR /*?*/ /*out*/ * wide_string
);


#endif
