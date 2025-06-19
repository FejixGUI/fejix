#include <fejix/utils/logging.h>

#include <inttypes.h>
#include <stdio.h>

static char const *log_level_to_string(enum fj_log_level level)
{
    switch (level) {
        default:
        case FJ_LOG_INFO:
            return "Info";

        case FJ_LOG_WARN:
            return "Warning";

        case FJ_LOG_ERROR:
            return "Error";
    }
}

static void default_log(
    enum fj_log_level log_level,
    char const *file,
    uint32_t line,
    char const *function,
    char const *message)
{
    fprintf(
        stderr,
        "[fejix log] %s at %s() in %s@%" PRIu32 ": %s\n",
        log_level_to_string(log_level),
        function,
        file,
        line,
        message);
}


void (*fj_log_callback)(
    enum fj_log_level log_level,
    char const *file,
    uint32_t line,
    char const *function,
    char const *message)
    = default_log;
