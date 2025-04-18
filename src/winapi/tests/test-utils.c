#include <src/winapi/utils.h>

#include <fejix/core/alloc.h>
#include <fejix/core/utils.h>

#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    LPWSTR wide_string = NULL;
    FJ_TRY (fj_winapi_into_utf16("", &wide_string)) {
        fprintf(stderr, "Failed: empty string: %s\n", enum fj_error_get_description(fj_result));
        return 1;
    }

    FJ_FREE(&wide_string);

    return 0;
}
