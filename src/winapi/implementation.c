#include <fejix/implementation.h>


extern struct fj_client_iface fj_winapi_client_impl;


struct fj_implementation const fj_winapi_implementation = {
    .id = FJ_IMPLEMENTATION_WINAPI,
    .version = FJ_VERSION(0, 0, 1),
    .client = &fj_winapi_client_impl,
    .wm = NULL,
};
