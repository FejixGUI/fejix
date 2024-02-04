#include <fejix/bus.h>
#include <fejix/utils.h>


#ifdef FJ_OPT_ANDK
    extern struct fj_bus const fj_andk_bus;
#endif
#ifdef FJ_OPT_COCOA
    extern struct fj_bus const fj_cocoa_bus;
#endif
#ifdef FJ_OPT_NOOP
    extern struct fj_bus const fj_noop_bus;
#endif
#ifdef FJ_OPT_UIKIT
    extern struct fj_bus const fj_uikit_bus;
#endif
#ifdef FJ_OPT_WAYLAND
    extern struct fj_bus const fj_wayland_bus;
#endif
#ifdef FJ_OPT_WINAPI
    extern struct fj_bus const fj_winapi_bus;
#endif
#ifdef FJ_OPT_X11
    extern struct fj_bus const fj_x11_bus;
#endif


static
struct fj_bus const * const bus_list[] = {

#ifdef FJ_OPT_ANDK
    &fj_andk_bus,
#endif
#ifdef FJ_OPT_COCOA
    &fj_cocoa_bus,
#endif
#ifdef FJ_OPT_NOOP
    &fj_noop_bus,
#endif
#ifdef FJ_OPT_UIKIT
    &fj_uikit_bus,
#endif
#ifdef FJ_OPT_WAYLAND
    &fj_wayland_bus,
#endif
#ifdef FJ_OPT_WINAPI
    &fj_winapi_bus,
#endif
#ifdef FJ_OPT_X11
    &fj_x11_bus,
#endif

};


void fj_get_buses(
    uint32_t FJ_OUT * bus_count,
    struct fj_bus const * const * FJ_ARRAY FJ_OUT * buses
)
{
    *bus_count = FJ_ARRLEN(bus_list);
    *buses = bus_list;
}
