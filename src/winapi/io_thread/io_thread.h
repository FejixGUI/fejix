#ifndef FEJIX_WINAPI_IO_THREAD_H_
#define FEJIX_WINAPI_IO_THREAD_H_


#include <src/winapi/io_thread/timer.h>
#include <src/winapi/window/window.h>

#include <fejix/modules/io_thread.h>

#include <windows.h>


struct fj_io_thread {
    void *userdata;

    fj_io_thread_event_callback callback;

    HINSTANCE instance;
    HWND message_window;

    bool finished;

    struct fj_io_thread_timer_manager timer_manager;

    struct fj_io_thread_sleep_manager {
        void *userdata;
        struct fj_io_thread *io_thread;
    } sleep_manager;
};


#endif
