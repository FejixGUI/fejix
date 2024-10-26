#ifndef FEJIX_WINAPI_CLIENT_H_
#define FEJIX_WINAPI_CLIENT_H_


#include <src/winapi/sleep_timer/sleep_timer.h>

#include <fejix/interface/client.h>

#include <windows.h>


enum fj_winapi_user_message_id {
    FJ_WINAPI_USER_MESSAGE_SLEEP = WM_USER,
    FJ_WINAPI_USER_MESSAGE_WAKEUP,
};


struct fj_winapi_window_data_base {
    struct fj_client *client;
};

struct fj_winapi_client_impl {
    struct fj_client_callbacks callbacks;

    HINSTANCE instance;

    HWND message_window;
    struct fj_winapi_window_data_base message_window_data;
    fj_err_t message_processing_result;

    fj_bool8_t is_idle_requested;
    fj_bool8_t is_quit_requested;
};


struct fj_client {
    union fj_tag tag;

    struct fj_winapi_client_impl client_impl;

    struct fj_sleep_timer_impl sleep_timer_impl;
};


LONG_PTR __stdcall fj_winapi_window_procedure(
    HWND window,
    UINT message,
    UINT_PTR wparam,
    LONG_PTR lparam
);


#endif
