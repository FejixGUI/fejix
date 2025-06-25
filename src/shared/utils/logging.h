#ifndef FEJIX_UTILS_LOGGING_H_
#define FEJIX_UTILS_LOGGING_H_


#include <fejix/utils.h>


#ifdef FJ_COMPILE_OPT_ENABLE_LOGGING
#    define FJ_LOG(LEVEL, ...) fj_log(LEVEL, __FILE__, __LINE__, __func__, __VA_ARGS__)
#else
// This prevents warnings about empty blocks
#    define FJ_LOG(LEVEL, ...) (void) 0;
#endif

#define FJ_ERROR(...) FJ_LOG(FJ_LOG_LEVEL_ERROR, __VA_ARGS__)
#define FJ_WARN(...) FJ_LOG(FJ_LOG_LEVEL_WARN, __VA_ARGS__)
#define FJ_INFO(...) FJ_LOG(FJ_LOG_LEVEL_INFO, __VA_ARGS__)

/** Calls fj_log_callback with the formatted message. */
void fj_log(
    enum fj_log_level log_level,
    char const *file,
    uint32_t line,
    char const *function,
    char const *format,
    ...);


#endif
