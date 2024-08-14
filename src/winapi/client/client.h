#include <fejix/interface/client.h>

#include <windows.h>


typedef uint32_t fj_winapi_event_handling_flags_t;

enum fj_winapi_event_handling_flags {
    FJ_WINAPI_INSIDE_DEFWINDOWPROC = (1<<0),
    FJ_WINAPI_POSTED_QUIT_MESSAGE = (1<<1),
};


struct fj_winapi_client {
    struct fj_client_callbacks const * callbacks;
    void * data;

    fj_seconds_t timeout;
    fj_err_t event_handling_error;
    fj_winapi_event_handling_flags_t event_handling_flags;

    HINSTANCE instance;
    DWORD main_thread_id;

};
