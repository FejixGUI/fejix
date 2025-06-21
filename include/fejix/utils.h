#ifndef FEJIX_UTILS_H_
#define FEJIX_UTILS_H_


#include <fejix/base.h>


enum fj_log_level {
    FJ_LOG_LEVEL_INFO,
    FJ_LOG_LEVEL_WARN,
    FJ_LOG_LEVEL_ERROR,

    FJ_LOG_LEVEL_ENUM32 = INT32_MAX,
};

FJ_PUBLIC
void (*fj_log_callback)(
    enum fj_log_level log_level,
    char const *file,
    uint32_t line,
    char const *function,
    char const *message);

/**
    \returns NULL on allocation failure or when freeing.
*/
FJ_PUBLIC
void *(*fj_alloc_callback)(void *pointer, size_t old_size, size_t new_size);


#endif
