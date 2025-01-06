#ifndef FEJIX_IMP_WINAPI_UTILS_H_
#define FEJIX_IMP_WINAPI_UTILS_H_


#include <fejix/core/base.h>

#include <windows.h>


struct fj_winapi_window_info {
    /** If NULL, DefWindowProc is used. */
    WNDPROC procedure;

    UINT class_style;
    DWORD style;
    DWORD exstyle;
    int x;
    int y;
    int width;
    int height;
    HWND parent;
    HMENU menu;
    LPVOID create_param;
};


/**
    Converts UTF-8 to Windows multibyte ("wide") encoding.
    The returned string must be freed manually.

    :param utf16_string: Returns the converted string or NULL.
    :returns: Error if the given string is invalid UTF-8.
*/
fj_err_t fj_winapi_into_utf16(char const *string, LPWSTR *utf16_string);

/** :param string: Returns the converted UTF-8 string or NULL. */
fj_err_t fj_winapi_from_utf16(LPWSTR utf16_string, char const **string);

/**
    Creates a simple generic window with its own window class on the current executable instance,
    sets as few default parameters as possible.
*/
fj_err_t fj_winapi_window_create_simple(
    HWND *out_window,
    struct fj_winapi_window_info const *window_info
);

/**
    Destroys a simple window and its window class on the current executable instance.
*/
fj_err_t fj_winapi_window_destroy_simple(HWND window);

void fj_winapi_window_set_data(HWND window, void *data);

void *fj_winapi_window_get_data(HWND window);


#endif
