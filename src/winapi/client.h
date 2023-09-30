#ifndef FEJIX_INTERNAL_WINAPI_CLIENT_H_
#define FEJIX_INTERNAL_WINAPI_CLIENT_H_


#include <src/generic/client.h>

#include <windows.h>


#ifdef FJ_FEATURE_SHELL
#   include <src/winapi/features/shell.h>
#endif


struct fj_winapi_data {
    HINSTANCE app_instance;

#ifdef FJ_FEATURE_SHELL
    struct fj_winapi_shell_data shell;
#endif
};


#endif
