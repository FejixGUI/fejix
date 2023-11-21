#include "src/winapi/utils.h"
#include <fejix/utils.h>
#include <fejix/malloc.h>

#include <stdio.h>
#include <stdlib.h>


int main(void) {
    FJ_INIT_ERRORS

    LPWSTR FJ_NULLABLE wide_string = NULL;

    FJ_TRY fj_winapi_utf8_to_wstr(FJ_UTF8(""), &wide_string);

    if (FJ_FAILED) {
        fprintf(stderr, "Failed to convert empty string: %s\n", FJ_LAST_ERROR);
        return 1;
    }

    fj_free(wide_string);

    return 0;
}
