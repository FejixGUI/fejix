#ifndef FEJIX_UTILS_LOGGING_H_INCLUDED
#define FEJIX_UTILS_LOGGING_H_INCLUDED


#include <fejix/core.h>

#ifdef FJ_COMPILE_OPT_ENABLE_LOGGING
#    define FJ_LOG(LEVEL, MESSAGE) fj_log_callback(LEVEL, __FILE__, __LINE__, __func__, MESSAGE)
#else
#    define FJ_LOG(LEVEL, MESSAGE)
#endif

#define FJ_ERROR(MESSAGE) FJ_LOG(FJ_LOG_LEVEL_ERROR, MESSAGE)
#define FJ_WARN(MESSAGE) FJ_LOG(FJ_LOG_LEVEL_WARN, MESSAGE)
#define FJ_INFO(MESSAGE) FJ_LOG(FJ_LOG_LEVEL_INFO, MESSAGE)


enum fj_log_level {
    FJ_LOG_INFO,
    FJ_LOG_WARN,
    FJ_LOG_ERROR,

    FJ_LOG_LEVEL_ENUM32 = INT32_MAX,
};


FJ_PUBLIC
void (*fj_log_callback)(
    enum fj_log_level log_level,
    char const *file,
    uint32_t line,
    char const *function,
    char const *message);


#endif
