#include <src/loader/loader.h>

#include <fejix/loader/loader.h>

#include <fejix/core/alloc.h>
#include <fejix/core/utils.h>

#undef UNICODE
#define UNICODE
#undef _UNICODE
#define _UNICODE
#undef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


static HMODULE library;


static fj_loader_function_t load_function(char const *function_name)
{
    return (fj_loader_function_t) GetProcAddress(library, function_name);
}


static void load_functions(void)
{
    for (uint32_t i = 0; i < fj_loader_function_count; i++) {
        *fj_loader_function_pointers[i] = load_function(fj_loader_function_names[i]);
    }
}

static void unload_functions(void)
{
    for (uint32_t i = 0; i < fj_loader_function_count; i++) {
        *fj_loader_function_pointers[i] = NULL;
    }
}


static fj_err_t load_library(LPCWSTR library_path)
{
    library = LoadLibrary(library_path);

    if (library == NULL) {
        return FJ_ERR_CANNOT_LOAD_LIBRARY;
    }

    load_functions();

    return FJ_OK;
}


static void unload_library(void)
{
    unload_functions();
    FreeLibrary(library);
}


static LPWSTR get_builtin_library_path(void)
{
    return TEXT("fejix_winapi.dll");
}


/** :returns: The path, needs to be freed. NULL on failure. */
static LPWSTR get_default_library_path(void)
{
    LPCWSTR varname = TEXT("FEJIX_DEFAULT_LIBRARY_PATH");

    DWORD length = GetEnvironmentVariable(varname, NULL, 0);

    if (length == 0) {
        return get_builtin_library_path();
    }

    LPWSTR buffer;
    FJ_TRY (FJ_REALLOC_UNINIT(&buffer, length)) {
        return get_builtin_library_path();
    }

    GetEnvironmentVariable(varname, buffer, length);

    return buffer;
}


static fj_err_t str_into_utf16(char const *string, LPWSTR *out_utf16_string)
{
    int output_char_count = MultiByteToWideChar(CP_UTF8, 0, string, -1, NULL, 0);

    FJ_TRY (FJ_REALLOC_UNINIT(out_utf16_string, output_char_count)) {
        return fj_result;
    }

    int result = MultiByteToWideChar(CP_UTF8, 0, string, -1, *out_utf16_string, output_char_count);

    if (result == 0) {
        FJ_FREE(out_utf16_string);
        return FJ_ERR_INVALID_TEXT_ENCODING;
    }

    return FJ_OK;
}


static fj_err_t get_library_path(char const *utf8_library_path, LPWSTR *out_library_path)
{
    if (utf8_library_path != NULL) {
        return str_into_utf16(utf8_library_path, out_library_path);
    }

    *out_library_path = get_default_library_path();
    return FJ_OK;
}


static void free_library_path(LPWSTR library_path)
{
    if (library_path != get_builtin_library_path()) {
        FJ_FREE(&library_path);
    }
}


fj_err_t fj_loader_load_library(char const *utf8_library_path)
{
    LPWSTR library_path;
    FJ_TRY (get_library_path(utf8_library_path, &library_path)) {
        return fj_result;
    }

    fj_err_t result = load_library(library_path);

    free_library_path(library_path);

    return result;
}


void fj_loader_unload_library(void)
{
    unload_library();
}
