#ifndef FEJIX_WINAPI_IO_THREAD_H_INCLUDED
#define FEJIX_WINAPI_IO_THREAD_H_INCLUDED


#include <fejix/modules/io_thread.h>

#include <windows.h>


struct fj_io_thread {
    void *userdata;
    fj_io_thread_event_callback callback;

    HINSTANCE instance;
    HWND message_only_window;
};


#endif
