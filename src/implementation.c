#include <fejix/implementation.h>

#include <fejix/core/utils.h>


static
char const */*[]*/ implementation_names[] = {
    [FJ_IMPLEMENTATION_MAX] = "...max implementation ID...",

    [FJ_IMPLEMENTATION_ANDK] = "andk",
    [FJ_IMPLEMENTATION_APPKIT] = "appkit",
    [FJ_IMPLEMENTATION_WAYLAND] = "wayland",
    [FJ_IMPLEMENTATION_WINAPI] = "winapi",
    [FJ_IMPLEMENTATION_X11] = "x11",
};


extern struct fj_implementation const fj_andk_implementation;
extern struct fj_implementation const fj_appkit_implementation;
extern struct fj_implementation const fj_wayland_implementation;
extern struct fj_implementation const fj_winapi_implementation;
extern struct fj_implementation const fj_x11_implementation;

#ifdef FJ_OPT_ANY_IMPLEMENTATION

    static
    struct fj_implementation const *const implementation_list[] = {

#   ifdef FJ_OPT_ANDK
        &fj_andk_implementation,
#   endif
#   ifdef FJ_OPT_APPKIT
        &fj_appkit_implementation,
#   endif
#   ifdef FJ_OPT_WAYLAND
        &fj_wayland_implementation,
#   endif
#   ifdef FJ_OPT_WINAPI
        &fj_winapi_implementation,
#   endif
#   ifdef FJ_OPT_X11
        &fj_x11_implementation,
#   endif

    };

#endif


char const */*[]?*/ fj_get_implementation_name(fj_implementation_id_t implementation_id)
{
    if (implementation_id >= FJ_IMPLEMENTATION_MAX) {
        return NULL;
    }

    if (implementation_names[implementation_id] == NULL) {
        return "some implementation (name not implemented yet)";
    }

    return implementation_names[implementation_id];
}


void fj_get_builtin_implementations(
    struct fj_implementation const *const */*[]? out*/ * implementations,
    uint32_t /*out*/ * implementation_count
)
{
#ifdef FJ_OPT_ANY_IMPLEMENTATION
    *implementations = implementation_list;
    *implementation_count = FJ_ARRAY_LEN(implementation_list);
#else
    *implementations = NULL;
    *implementation_count = 0;
#endif
}
