#include <fejix/implementation.h>

#include <fejix/core/utils.h>


#if defined(FJ_OPT_ANDK) || defined(FJ_OPT_COCOA) || defined(FJ_OPT_WAYLAND) \
    || defined(FJ_OPT_WINAPI) || defined(FJ_OPT_X11)
#    define ANY_IMPLEMENTATION
#endif


extern struct fj_implementation const fj_andk_implementation;
extern struct fj_implementation const fj_cocoa_implementation;
extern struct fj_implementation const fj_wayland_implementation;
extern struct fj_implementation const fj_winapi_implementation;
extern struct fj_implementation const fj_x11_implementation;

#ifdef ANY_IMPLEMENTATION

static struct fj_implementation const *const implementation_list[] = {

#    ifdef FJ_OPT_ANDK
    &fj_andk_implementation,
#    endif
#    ifdef FJ_OPT_COCOA
    &fj_cocoa_implementation,
#    endif
#    ifdef FJ_OPT_WAYLAND
    &fj_wayland_implementation,
#    endif
#    ifdef FJ_OPT_WINAPI
    &fj_winapi_implementation,
#    endif
#    ifdef FJ_OPT_X11
    &fj_x11_implementation,
#    endif

};

#endif


void fj_get_builtin_implementations(
    struct fj_implementation const *const **implementations,
    uint32_t *implementation_count
)
{
#ifdef ANY_IMPLEMENTATION
    *implementations = implementation_list;
    *implementation_count = FJ_ARRAY_LEN(implementation_list);
#else
    *implementations = NULL;
    *implementation_count = 0;
#endif
}
