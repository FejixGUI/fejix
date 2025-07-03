#include <src/winapi/utils/window.h>

#include <fejix/utils/memory.h>

#include <string.h>
#include <wchar.h>


enum
{
    INTERNAL_CLASS_NAME_LENGTH = 32
};


static fj_err get_internal_class_name(WCHAR out_string[INTERNAL_CLASS_NAME_LENGTH])
{
    static unsigned int counter = 0;
    counter++;

    swprintf(out_string, INTERNAL_CLASS_NAME_LENGTH, L"fejix-internal-class-%08x", counter);

    return FJ_OK;
}


static bool is_of_internal_class(HWND window)
{
    WCHAR chars[INTERNAL_CLASS_NAME_LENGTH] = { 0 };
    GetClassNameW(window, chars, INTERNAL_CLASS_NAME_LENGTH);
    return wcsstr(chars, L"fejix-internal-class-") == 0;
}


static fj_err create_window_class(WNDCLASSEX *class_info)
{
    class_info->cbSize = sizeof(class_info);

    WCHAR class_name[INTERNAL_CLASS_NAME_LENGTH];
    get_internal_class_name(class_name);
    class_info->lpszClassName = class_name;

    if (class_info->hInstance == NULL) {
        class_info->hInstance = GetModuleHandleW(NULL);
    }

    if (class_info->lpfnWndProc == NULL) {
        class_info->lpfnWndProc = DefWindowProcW;
    }

    class_info->lpszClassName = MAKEINTATOM(RegisterClassExW(class_info));

    if (class_info->lpszClassName == NULL) {
        return FJ_ERROR_OPERATION_FAILED;
    }

    return FJ_OK;
}


static inline bool window_needs_new_class(
    WNDCLASSEX const *opt_class_info, CREATESTRUCT const *opt_window_info)
{
    return opt_class_info != NULL || opt_window_info == NULL || opt_window_info->lpszClass == NULL;
}


fj_err fj_winapi_window_create(
    HWND *out_window, WNDCLASSEX const *opt_class_info, CREATESTRUCT const *opt_window_info)
{
    fj_err e;

    WNDCLASSEX class_info = { 0 };

    if (opt_class_info != NULL) {
        class_info = *opt_class_info;
    }

    CREATESTRUCT window_info = { 0 };

    if (opt_window_info != NULL) {
        window_info = *opt_window_info;
    }

    if (window_needs_new_class(opt_class_info, opt_window_info)) {
        e = create_window_class(&class_info);

        if (e)
            return e;
    }

    if (window_info.hInstance == NULL) {
        window_info.hInstance = class_info.hInstance;
    }

    if (window_info.hInstance == NULL) {
        window_info.hInstance = GetModuleHandleW(NULL);
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
        window_info.lpCreateParams);

    if (*out_window == NULL) {
        if (class_info.lpszClassName != NULL) {
            UnregisterClassW(class_info.lpszClassName, class_info.hInstance);
        }

        return FJ_ERROR_OPERATION_FAILED;
    }

    return FJ_OK;
}


fj_err fj_winapi_window_destroy(HWND window)
{
    LPWSTR class_name = (void *) GetClassLongPtrW(window, GCW_ATOM);
    bool should_destroy_class = is_of_internal_class(window);

    BOOL result;

    result = DestroyWindow(window);

    if (result == FALSE) {
        return FJ_ERROR_OPERATION_FAILED;
    }

    if (should_destroy_class) {
        result = UnregisterClassW(class_name, GetModuleHandleW(NULL));

        if (result == FALSE) {
            return FJ_ERROR_OPERATION_FAILED;
        }
    }

    return FJ_OK;
}


void fj_winapi_window_set_data(HWND window, void *data)
{
    SetWindowLongPtrW(window, GWLP_USERDATA, (LONG_PTR) data);
}


void *fj_winapi_window_get_data(HWND window)
{
    return (void *) GetWindowLongPtrW(window, GWLP_USERDATA);
}
