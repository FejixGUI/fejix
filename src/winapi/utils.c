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


static fj_err_t get_class_name(WCHAR out_string[64])
{
    SYSTEMTIME time;
    GetSystemTime(&time);

    static unsigned int counter = 0;
    counter++;

    swprintf(
        out_string,
        64,
        L"fejix-window-class-%04hx%04hx%04hx%04hx%04hx%04hx%04hx%08x",
        time.wYear,
        time.wMonth,
        time.wDay,
        time.wHour,
        time.wMinute,
        time.wSecond,
        time.wMilliseconds,
        counter
    );

    return FJ_OK;
}


fj_err_t create_window_class(
    struct fj_winapi_window_info const *window_info,
    LPWSTR *out_class_name
)
{
    WCHAR class_name[64];
    get_class_name(class_name);

    WNDCLASSEX info = {
        .cbSize = sizeof(info),
        .lpszClassName = class_name,
        .hInstance = GetModuleHandle(NULL),
        .style = window_info->class_style,
        .lpfnWndProc = window_info->procedure ? window_info->procedure : DefWindowProc,
    };

    *out_class_name = MAKEINTATOM(RegisterClassEx(&info));

    if (*out_class_name == NULL) {
        return FJ_ERR_REQUEST_FAILED;
    }

    return FJ_OK;
}


fj_err_t fj_winapi_window_create_simple(
    HWND *out_window,
    struct fj_winapi_window_info const *window_info
)
{
    LPWSTR class_name;
    FJ_TRY (create_window_class(window_info, &class_name)) {
        return fj_result;
    }

    *out_window = CreateWindowEx(
        window_info->exstyle,
        class_name,
        NULL,
        window_info->style,
        window_info->x,
        window_info->y,
        window_info->width,
        window_info->height,
        window_info->parent,
        window_info->menu,
        GetModuleHandle(NULL),
        window_info->create_param
    );

    if (*out_window == NULL) {
        UnregisterClass(class_name, GetModuleHandle(NULL));
        return FJ_ERR_REQUEST_FAILED;
    }

    return FJ_OK;
}


fj_err_t fj_winapi_window_destroy_simple(HWND window)
{
    LPWSTR class_name = (void *) GetClassLongPtr(window, GCW_ATOM);

    if (DestroyWindow(window) == 0) {
        return FJ_ERR_REQUEST_FAILED;
    }

    if (UnregisterClass(class_name, GetModuleHandle(NULL)) == 0) {
        return FJ_ERR_REQUEST_FAILED;
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
