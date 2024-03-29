#include <src/winapi/utils.h>

#include <fejix/malloc.h>
#include <fejix/utils.h>

#include <string.h>


fj_err_t fj_winapi_utf8_to_wstr(
    fj_string_t utf8_string,
    LPWSTR FJ_NULLABLE FJ_OUT * wide_string
)
{
    FJ_INIT_ERRORS

    char const * c_string = (char const *) utf8_string;

    int32_t output_chars_count = MultiByteToWideChar(
        CP_UTF8,
        0, /* flags */
        c_string,
        -1, /* convert the entire string */
        NULL, /* output string */
        0 /* output chars count (unknown, asking for it) */
    );

    FJ_TRY fj_alloc_uninit(
        (void *) wide_string,
        output_chars_count * sizeof(WCHAR)
    );

    if (FJ_FAILED) {
        return FJ_LAST_ERROR;
    }

    uint32_t result = MultiByteToWideChar(
        CP_UTF8,
        0, /* flags */
        c_string,
        -1, /* convert the entire string */
        *wide_string,
        output_chars_count
    );

    if (result == 0) {
        fj_free((void *) wide_string);
        return FJ_ERR("cannot convert invalid UTF-8 to wide string");
    }

    return FJ_OK;
}
