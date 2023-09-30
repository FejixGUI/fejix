#ifndef FJ_INTERNAL_WINAPI_SHELL_H_
#define FJ_INTERNAL_WINAPI_SHELL_H_


#include <src/generic/shell.h>

#include <windows.h>


struct fj_winapi_shell_data {
    LPWSTR window_class_name;
};


fj_err_t fj_winapi_shell_init(struct fj_client * client);
fj_err_t fj_winapi_shell_deinit(struct fj_client * client);


#endif
