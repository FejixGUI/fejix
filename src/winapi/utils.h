#ifndef FEJIX_WINAPI_UTILS_H_
#define FEJIX_WINAPI_UTILS_H_


#include <fejix/core/base.h>
#include <windows.h>


/** Converts UTF-8 to Windows multibyte ("wide") encoding.
    The returned string must be freed with `fj_free`.

    Returns an error if the given string is invalid UTF-8. */
fj_err_t fj_winapi_into_utf16(uint8_t const */*[]*/ string, LPWSTR /*? out*/ * utf16_string);

fj_err_t fj_winapi_from_utf16(LPWSTR utf16_string, uint8_t const */*[] out*/ * string);


#endif
