#include "logging.h"

#include <src/shared/utils/memory.h>

#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>


static char const *log_level_to_string(enum fj_log_level level)
{
    switch (level) {
        default:
        case FJ_LOG_LEVEL_INFO:
            return "Info";

        case FJ_LOG_LEVEL_WARN:
            return "Warning";

        case FJ_LOG_LEVEL_ERROR:
            return "Error";
    }
}

static void default_callback(
    enum fj_log_level log_level,
    char const *file,
    uint32_t line,
    char const *function,
    char const *message)
{
    fprintf(
        stderr,
        "[fejix log] %s:%" PRIu32 ": %s at %s(): %s\n",
        file,
        line,
        log_level_to_string(log_level),
        function,
        message);
}


void (*fj_log_callback)(
    enum fj_log_level log_level,
    char const *file,
    uint32_t line,
    char const *function,
    char const *message)
    = default_callback;

void fj_log(
    enum fj_log_level log_level,
    char const *file,
    uint32_t line,
    char const *function,
    char const *format,
    ...)
{
    char message[256];
    va_list args;
    va_start(args, format);
    vsnprintf(message, FJ_LEN(message), format, args);
    va_end(args);

    fj_log_callback(log_level, file, line, function, message);
}
