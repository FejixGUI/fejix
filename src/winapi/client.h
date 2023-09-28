#ifndef FEJIX_INTERNAL_WINAPI_CLIENT_H_
#define FEJIX_INTERNAL_WINAPI_CLIENT_H_


#include <src/generic/client.h>

#include <windows.h>


struct fj_winapi_data {
    HINSTANCE app_instance;
    LPWSTR window_class_name;
};


#ifdef FJ_FEATURE_SHELL
    fj_err_t fj_winapi_shell_init(struct fj_client * client);
    fj_err_t fj_winapi_shell_deinit(struct fj_client * client);
#endif


#endif
