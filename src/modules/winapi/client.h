#ifndef FEJIX_INTERNAL_WINAPI_CLIENT_H_
#define FEJIX_INTERNAL_WINAPI_CLIENT_H_


#include <fejix/client.h>

#include <windows.h>


struct fj_winapi_data {
    HINSTANCE app_instance;
    LPWSTR window_class_name;
};


#endif
