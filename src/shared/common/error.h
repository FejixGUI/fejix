#ifndef FEJIX_COMMON_ERROR_H_
#define FEJIX_COMMON_ERROR_H_


#include <fejix/base.h>


#ifdef FJ_OPT_ENABLE_ERRORS
#    ifdef FJ_OPT_ENABLE_ERROR_LOCATIONS
#        define FJ_ERROR(MESSAGE) \
            fj_format_error(      \
                "[%s:%d@%s] %s", __FILE__, __LINE__, __func__, MESSAGE)
#        define FJ_ERRORF(FORMAT, ...) \
            fj_format_error(           \
                "[%s:%d@%s] " FORMAT,  \
                __FILE__,              \
                __LINE__,              \
                __func__,              \
                __VA_ARGS__)
#    else
#        define FJ_ERROR(MESSAGE) fj_format_error("[%s] %s", __func__, MESSAGE)
#        define FJ_ERRORF(FORMAT, ...) \
            fj_format_error("[%s] " FORMAT, __func__, __VA_ARGS__)
#    endif
#else
// The expansion prevents warnings about empty blocks and identical if-else
// branches
#    define FJ_ERROR(...) (void) __LINE__;
#    define FJ_ERRORF(...) (void) __LINE__;
#endif


void fj_format_error(char const *format, ...);


#endif
