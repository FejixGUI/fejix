#include <src/winapi/utils.h>

#include <fejix/core/utils.h>

#include <string.h>
#include <wchar.h>


fj_err_t fj_winapi_into_utf16(char const *string, LPWSTR *utf16_string)
{
    int32_t output_char_count = MultiByteToWideChar(
        CP_UTF8,
        0, /* flags */
        string,
        -1,   /* convert the entire string */
        NULL, /* output string */
        0     /* output chars count (unknown, asking for it) */
    );

    FJ_TRY (FJ_REALLOC_UNINIT(utf16_string, output_char_count)) {
        return fj_result;
    }

    uint32_t result = MultiByteToWideChar(
        CP_UTF8,
        0, /* flags */
        string,
        -1, /* convert the entire string */
        *utf16_string,
        output_char_count
    );

    if (result == 0) {
        FJ_FREE(utf16_string);
        return FJ_ERR_INVALID_TEXT_ENCODING;
    }

    return FJ_OK;
}


fj_err_t fj_winapi_from_utf16(LPWSTR utf16_string, char const **string)
{
    int32_t output_size = WideCharToMultiByte(
        CP_UTF8,
        0, /* flags */
        utf16_string,
        -1,   /* convert the entire string */
        NULL, /* output string */
        0,    /* output chars count (unknown, asking for it) */
        NULL,
        NULL
    );

    FJ_TRY (FJ_REALLOC_UNINIT(string, output_size)) {
        return fj_result;
    }

    uint32_t result = WideCharToMultiByte(
        CP_UTF8,
        0, /* flags */
        utf16_string,
        -1, /* convert the entire string */
        (char *) *string,
        output_size,
        NULL,
        NULL
    );

    if (result == 0) {
        FJ_FREE(string);
        return FJ_ERR_INVALID_TEXT_ENCODING;
    }

    return FJ_OK;
}


static fj_err_t get_junk_class_name(WCHAR out_string[32])
{
    static unsigned int counter = 0;
    counter++;

    swprintf(out_string, 32, L"fejix-junk-window-class%08x", counter);

    return FJ_OK;
}


static bool is_of_junk_class(HWND window)
{
    WCHAR chars[32] = { 0 };
    GetClassName(window, chars, 32);
    return wcsstr(chars, L"fejix-junk-window-class") == 0;
}


static fj_err_t create_window_class(WNDCLASSEX *class_info)
{
    class_info->cbSize = sizeof(class_info);

    WCHAR class_name[32];
    get_junk_class_name(class_name);
    class_info->lpszClassName = class_name;

    if (class_info->hInstance == NULL) {
        class_info->hInstance = GetModuleHandle(NULL);
    }

    if (class_info->lpfnWndProc == NULL) {
        class_info->lpfnWndProc = DefWindowProc;
    }

    class_info->lpszClassName = MAKEINTATOM(RegisterClassEx(class_info));

    if (class_info->lpszClassName == NULL) {
        return FJ_ERR_REQUEST_FAILED;
    }

    return FJ_OK;
}


static inline bool window_needs_new_class(
    WNDCLASSEX const *maybe_class_info,
    CREATESTRUCT const *maybe_window_info
)
{
    return maybe_class_info != NULL || maybe_window_info == NULL
        || maybe_window_info->lpszClass == NULL;
}


fj_err_t fj_winapi_window_create(
    HWND *out_window,
    WNDCLASSEX const *maybe_class_info,
    CREATESTRUCT const *maybe_window_info
)
{
    WNDCLASSEX class_info = { 0 };

    if (maybe_class_info != NULL) {
        class_info = *maybe_class_info;
    }

    CREATESTRUCT window_info = { 0 };

    if (maybe_window_info != NULL) {
        window_info = *maybe_window_info;
    }

    if (window_needs_new_class(maybe_class_info, maybe_window_info)) {
        FJ_TRY (create_window_class(&class_info)) {
            return fj_result;
        }
    }

    if (window_info.hInstance == NULL) {
        window_info.hInstance = class_info.hInstance;
    }

    if (window_info.hInstance == NULL) {
        window_info.hInstance = GetModuleHandle(NULL);
    }

    if (window_info.lpszClass == NULL) {
        window_info.lpszClass = class_info.lpszClassName;
    }

    *out_window = CreateWindowEx(
        window_info.dwExStyle,
        window_info.lpszClass,
        window_info.lpszName,
        window_info.style,
        window_info.x,
        window_info.y,
        window_info.cx,
        window_info.cy,
        window_info.hwndParent,
        window_info.hMenu,
        window_info.hInstance,
        window_info.lpCreateParams
    );

    if (*out_window == NULL) {
        if (class_info.lpszClassName != NULL) {
            UnregisterClass(class_info.lpszClassName, class_info.hInstance);
        }

        return FJ_ERR_REQUEST_FAILED;
    }

    return FJ_OK;
}


fj_err_t fj_winapi_window_destroy(HWND window)
{
    LPWSTR class_name = (void *) GetClassLongPtr(window, GCW_ATOM);
    bool should_destroy_class = is_of_junk_class(window);

    if (DestroyWindow(window) == 0) {
        return FJ_ERR_REQUEST_FAILED;
    }

    if (should_destroy_class) {
        if (UnregisterClass(class_name, GetModuleHandle(NULL)) == 0) {
            return FJ_ERR_REQUEST_FAILED;
        }
    }

    return FJ_OK;
}


void fj_winapi_window_set_data(HWND window, void *data)
{
    SetWindowLongPtr(window, GWLP_USERDATA, (LONG_PTR) data);
}


void *fj_winapi_window_get_data(HWND window)
{
    return (void *) GetWindowLongPtr(window, GWLP_USERDATA);
}
