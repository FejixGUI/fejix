#include <src/winapi/utils/encoding.h>

#include <fejix/utils/memory.h>


fj_err fj_winapi_into_utf16(char const *string, LPWSTR *utf16_string)
{
    fj_err e;

    int32_t output_chars_length = MultiByteToWideChar(
        CP_UTF8,
        0,  // flags
        string,
        -1,    // convert the entire string
        NULL,  // output string
        0      // output chars count (unknown, asking for it)
    );

    e = FJ_REALLOC_UNINIT(utf16_string, output_chars_length);

    if (e)
        return e;

    uint32_t result = MultiByteToWideChar(
        CP_UTF8,
        0,  // flags
        string,
        -1,  // convert the entire string
        *utf16_string,
        output_chars_length);

    if (result == 0) {
        FJ_FREE(utf16_string);
        return FJ_ERROR_INVALID_ENCODING;
    }

    return FJ_OK;
}


fj_err fj_winapi_from_utf16(LPWSTR utf16_string, char const **string)
{
    fj_err e;

    int32_t output_size = WideCharToMultiByte(
        CP_UTF8,
        0,  // flags
        utf16_string,
        -1,    // convert the entire string
        NULL,  // output string
        0,     // output chars count (unknown, asking for it)
        NULL,
        NULL);

    e = FJ_REALLOC_UNINIT(string, output_size);

    if (e)
        return e;

    uint32_t result = WideCharToMultiByte(
        CP_UTF8,
        0,  // flags
        utf16_string,
        -1,  // convert the entire string
        (char *) *string,
        output_size,
        NULL,
        NULL);

    if (result == 0) {
        FJ_FREE(string);
        return FJ_ERROR_INVALID_ENCODING;
    }

    return FJ_OK;
}
