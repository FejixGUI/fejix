#ifndef FJ_WINAPI_BUS_H_
#define FJ_WINAPI_BUS_H_


#include <fejix/base.h>
#include <fejix/bus.h>

#include <windows.h>


#define FJ_WINAPI_BUS_CONTEXT(STATE) ((struct fj_winapi_bus_context *)(STATE))

struct fj_winapi_bus_context {
    struct fj_bus_context_base base;

    HINSTANCE instance;
};


#endif
