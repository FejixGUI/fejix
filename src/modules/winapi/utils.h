#ifndef FEJIX_INTERNAL_WINAPI_UTILS_H_
#define FEJIX_INTERNAL_WINAPI_UTILS_H_


#include "fejix/core/base.h"
#include <windows.h>
#include <winnt.h>


/* Converts UTF-8 to Windows multibyte encoding.
    The returned string must be freed with `fj_free`.
    # Returns
    NULL if the given string is invalid:
    * the length of the given string is 0
    * the given string contains invlaid Unicode */
LPWSTR fj_winapi_utf8_to_wstr(fj_utf8string_t utf8_string);


#endif
