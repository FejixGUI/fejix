#ifndef FEJIX_UTILS_LOGGING_H_
#define FEJIX_UTILS_LOGGING_H_


#include <fejix/runtime.h>


#ifdef FJ_COMPILE_OPT_ENABLE_LOGGING
#    define FJ_LOG(LEVEL, MESSAGE) fj_log_callback(LEVEL, __FILE__, __LINE__, __func__, MESSAGE)
#else
// This prevents warnings about empty blocks
#    define FJ_LOG(LEVEL, MESSAGE) (void) MESSAGE;
#endif

#define FJ_ERROR(MESSAGE) FJ_LOG(FJ_LOG_LEVEL_ERROR, MESSAGE)
#define FJ_WARN(MESSAGE) FJ_LOG(FJ_LOG_LEVEL_WARN, MESSAGE)
#define FJ_INFO(MESSAGE) FJ_LOG(FJ_LOG_LEVEL_INFO, MESSAGE)


#endif
