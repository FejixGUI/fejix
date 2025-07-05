#include "error.h"

#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>


static void default_callback(char const *message)
{
#ifndef NDEBUG
    fprintf(stderr, "Fejix error: %s\n", message);
#else
    (void) message;
#endif
}

void (*fj_error_callback)(char const *message) = default_callback;


void fj_format_error(char const *format, ...)
{
    char message[128];
    va_list args;
    va_start(args, format);
    vsnprintf(message, FJ_LEN(message), format, args);
    va_end(args);

    fj_error_callback(message);
}
