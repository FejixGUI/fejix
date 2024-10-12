#include "src/winapi/utils.h"

#include <fejix/core/alloc.h>
#include <fejix/core/utils.h>

#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    LPWSTR wide_string = NULL;

    FJ_TRY (fj_winapi_into_utf16("", &wide_string)) {
        fprintf(stderr, "Failed to convert empty string: %u\n", fj_result);
        return 1;
    }

    FJ_FREE(&wide_string);

    return 0;
}
