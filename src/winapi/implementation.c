#include <fejix/implementation.h>


extern struct fj_app_class const fj_winapi_app_class;

struct fj_implementation const fj_winapi_implementation = {
    .id = FJ_IMPLEMENTATION_WINAPI,
    .api_version = { 0, 0, 1, },
    .app_class = &fj_winapi_app_class,
};
