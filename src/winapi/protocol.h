#ifndef FJ_WINAPI_PROTOCOL_H_
#define FJ_WINAPI_PROTOCOL_H_


#include <fejix/base.h>

#include <windows.h>



#define STATE(STATE) ((struct fj_winapi_state *)(STATE))

struct fj_winapi_state {
    void * FJ_NULLABLE callback_data;
};


#endif
