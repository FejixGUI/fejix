#ifndef FEJIX_WINAPI_UTILS_ENCODING_H_
#define FEJIX_WINAPI_UTILS_ENCODING_H_


#include <fejix/base.h>

#include <windows.h>


/** Converts UTF-8 to Windows multibyte ("wide") encoding.
    The returned string must be freed manually.

    \param[out] out_utf16_string Returns the converted string or NULL.
    \returns Error if the given string is invalid UTF-8.
*/
enum fj_status fj_winapi_into_utf16(char const *string, LPWSTR *out_utf16_string);

/** \param[out] out_string Returns the converted UTF-8 string or NULL on failure. */
enum fj_status fj_winapi_from_utf16(LPWSTR utf16_string, char const **out_string);


#endif
