#ifndef FEJIX_WINAPI_CLIENT_H_
#define FEJIX_WINAPI_CLIENT_H_


#include <src/winapi/sleep_timer/sleep_timer.h>

#include <fejix/interface/client.h>

#include <windows.h>


enum fj_winapi_user_message_id {
    FJ_WINAPI_USER_MESSAGE_SLEEP = WM_USER,
    FJ_WINAPI_USER_MESSAGE_WAKEUP,
};


struct fj_client {
    union fj_tag tag;
    struct fj_client_callbacks callbacks;

    HINSTANCE instance;

    HWND message_window;
    fj_err_t message_processing_result;

    fj_bool8_t is_idle_requested;
    fj_bool8_t is_quit_requested;

    struct fj_sleep_timer_manager sleep_timer_manager;
};


LONG_PTR fj_winapi_client_handle_message_safely(
    struct fj_client *client,
    MSG const *message,
    fj_err_t (*handle_message)(struct fj_client *client, MSG const *message, LONG_PTR *result)
);


#endif
