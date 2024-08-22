#include <src/winapi/utils.h>

#include <fejix/core/alloc.h>
#include <fejix/core/utils.h>

#include <string.h>


fj_err_t fj_winapi_into_utf16(char const */*[]*/ string, LPWSTR /*? out*/ * utf16_string)
{
    FJ_INIT_TRY

    int32_t output_char_count = MultiByteToWideChar(
        CP_UTF8,
        0, /* flags */
        string,
        -1, /* convert the entire string */
        NULL, /* output string */
        0 /* output chars count (unknown, asking for it) */
    );

    FJ_TRY(FJ_ARRALLOC_UNINIT(utf16_string, output_char_count)) {
        return FJ_RESULT;
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


fj_err_t fj_winapi_from_utf16(LPWSTR utf16_string, char const */*[] out*/ * string)
{
    FJ_INIT_TRY

    int32_t output_size = WideCharToMultiByte(
        CP_UTF8,
        0, /* flags */
        utf16_string,
        -1, /* convert the entire string */
        NULL, /* output string */
        0, /* output chars count (unknown, asking for it) */
        NULL,
        NULL
    );

    FJ_TRY(FJ_ARRALLOC_UNINIT(string, output_size)) {
        return FJ_RESULT;
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
