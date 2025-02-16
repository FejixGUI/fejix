#include <fejix/core/alloc.h>
#include <fejix/core/library.h>
#include <fejix/core/utils.h>


#ifdef _WIN32


#    undef UNICODE
#    define UNICODE
#    undef _UNICODE
#    define _UNICODE
#    undef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#    include <windows.h>

static fj_err_t str_into_utf16(char const *string, LPWSTR *out_utf16_string)
{
    int output_char_count = MultiByteToWideChar(CP_UTF8, 0, string, -1, NULL, 0);

    FJ_TRY (FJ_REALLOC_UNINIT(out_utf16_string, output_char_count)) {
        return fj_result;
    }

    int result = MultiByteToWideChar(CP_UTF8, 0, string, -1, *out_utf16_string, output_char_count);

    if (result == 0) {
        FJ_FREE(out_utf16_string);
        return FJ_ERR_INVALID_ENCODING;
    }

    return FJ_OK;
}

static fj_err_t library_load_utf8(
    struct fj_library *out_library, char const *utf8_path, fj_library_load_flags_t flags)
{
    LPWSTR utf16_path = NULL;
    FJ_TRY (str_into_utf16(utf8_path, &utf16_path)) {
        return fj_result;
    }

    flags &= FJ_LIBRARY_SYSTEM_ENCODED_PATH;

    FJ_TRY (fj_library_load(out_library, (void *) utf16_path, flags)) {
        FJ_FREE(&utf16_path);
        return fj_result;
    }

    FJ_FREE(&utf16_path);
    return FJ_OK;
}

static fj_library_function_t load_function(void *library_data, char const *function_name)
{
    return (fj_library_function_t) GetProcAddress(library_data, function_name);
}

fj_err_t fj_library_load(
    struct fj_library *out_library, char const *path, fj_library_load_flags_t flags)
{
    if (!(flags & FJ_LIBRARY_SYSTEM_ENCODED_PATH)) {
        return library_load_utf8(out_library, path, flags);
    }

#    if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)

    out_library->library_data = LoadLibrary((void *) path);

#    else

    // LoadLibrary is not available on UWP (Universal Windows Platform) used by Microsoft Store
    out_library->library_data = LoadPackagedLibrary((void *) path, 0);

#    endif

    if (out_library->library_data == NULL) {
        return FJ_ERR_CANNOT_LOAD_LIBRARY;
    }

    out_library->load_function = load_function;

    return FJ_OK;
}

void fj_library_unload(struct fj_library *library)
{
    FreeLibrary(library->library_data);
    *library = (struct fj_library) { 0 };
}


#else /* POSIX */


#    include <dlfcn.h>

static fj_library_function_t load_function(void *library_data, char const *function_name)
{
    return (fj_library_function_t) dlsym(library_data, function_name);
}

fj_err_t fj_library_load(
    struct fj_library *out_library, char const *path, fj_library_load_flags_t flags)
{
    int dl_flags = 0;
    dl_flags |= (flags & FJ_LIBRARY_LOAD_GLOBAL) ? RTLD_GLOBAL : 0;
    dl_flags |= (flags & FJ_LIBRARY_LOAD_LAZY) ? RTLD_LAZY : RTLD_NOW;

    out_library->library_data = dlopen(path, dl_flags);

    if (out_library->library_data == NULL) {
        return FJ_ERR_CANNOT_LOAD_LIBRARY;
    }

    out_library->load_function = load_function;

    return FJ_OK;
}

void fj_library_unload(struct fj_library *library)
{
    dlclose(library->library_data);
    *library = (struct fj_library) { 0 };
}


#endif
