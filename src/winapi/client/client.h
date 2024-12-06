#ifndef FEJIX_WINAPI_CLIENT_H_
#define FEJIX_WINAPI_CLIENT_H_


#include <fejix/interface/client.h>
#include <fejix/interface/client_wait_timeout.h>

#include <windows.h>


struct fj_client {
    union fj_tag tag;
    struct fj_client_callbacks callbacks;

    HINSTANCE instance;

    /** 0 means unset */
    fj_timeout_t wait_timeout;

    HWND message_window;

    fj_bool8_t is_idle_requested;
    fj_bool8_t is_quit_requested;
};


LONG_PTR fj_winapi_client_handle_message_safely(
    struct fj_client *client,
    MSG const *message,
    fj_err_t (*handle_message)(struct fj_client *client, MSG const *message, LONG_PTR *result)
);


#endif
