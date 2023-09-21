#include "src/modules/winapi/utils.h"

#include <fejix/core/malloc.h>
#include <fejix/core/utils.h>
#include <stringapiset.h>


LPWSTR fj_winapi_utf8_to_wstr(fj_utf8string_t utf8_string)
{
    uint32_t output_chars_count = MultiByteToWideChar(
        CP_UTF8,
        0, /* flags */
        utf8_string,
        -1, /* convert the entire string */
        NULL, /* output string */
        0 /* output chars count (unknown, asking for it) */
    );

    LPWSTR wide_string = fj_alloc_uninit(output_chars_count * sizeof(WCHAR));

    if (wide_string == NULL) {
        return NULL;
    }

    uint32_t result = MultiByteToWideChar(
        CP_UTF8,
        0, /* flags */
        utf8_string,
        -1, /* convert the entire string */
        wide_string,
        output_chars_count
    );

    if (result == 0) {
        fj_free(wide_string);
        return NULL; /* Invalid Unicode */
    }

    return wide_string;
}
