#ifndef FEJIX_WINAPI_UTILS_H_
#define FEJIX_WINAPI_UTILS_H_


#include <fejix/core/base.h>

#include <windows.h>


/**
Converts UTF-8 to Windows multibyte ("wide") encoding.
The returned string must be freed manually.

:param utf16_string: Returns the converted string or NULL.
:returns: Error if the given string is invalid UTF-8.
*/
fj_err_t fj_winapi_into_utf16(char const *string, LPWSTR *utf16_string);

/**
:param string: Returns the converted UTF-8 string or NULL.
*/
fj_err_t fj_winapi_from_utf16(LPWSTR utf16_string, char const **string);

void fj_winapi_set_window_data(HWND window, void *data);

void *fj_winapi_get_window_data(HWND window);


#endif
