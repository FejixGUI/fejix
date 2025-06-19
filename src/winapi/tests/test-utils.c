#include <src/winapi/utils/encoding.h>

#include <fejix/utils/memory.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    enum fj_status e;

    {
        LPWSTR wide_string = NULL;
        e = fj_winapi_into_utf16("", &wide_string);

        if (e) {
            fprintf(stderr, "Failed to convert empty string: %s\n", fj_status_get_description(e));
            return 1;
        }

        FJ_FREE(&wide_string);
    }

    {
        LPWSTR wide_string = NULL;
        e = fj_winapi_into_utf16("Γειά σου", &wide_string);

        if (e) {
            fprintf(
                stderr, "Failed to convert Greek characters: %s\n", fj_status_get_description(e));
            return 1;
        }

        assert(wcscmp(wide_string, L"\u0393\u03b5\u03b9\u03ac \u03c3\u03bf\u03c5") == 0);

        FJ_FREE(&wide_string);
    }

    {
        LPWSTR wide_string = NULL;
        e = fj_winapi_into_utf16("你好", &wide_string);

        if (e) {
            fprintf(
                stderr, "Failed to convert Chinese characters: %s\n", fj_status_get_description(e));
            return 1;
        }

        assert(wcscmp(wide_string, L"\u4f60\u597d") == 0);

        FJ_FREE(&wide_string);
    }

    return 0;
}
