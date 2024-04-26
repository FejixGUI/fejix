#include <src/wayland/bus.h>

#include <fejix/utils.h>
#include <fejix/malloc.h>


static
fj_err_t bus_open(
    void *fjOPTION fjOUT * bus_context
)
{
    FJ_INIT_ERRORS

    FJ_TRY fj_alloc_zeroed(bus_context, sizeof(struct fj_wayland_bus_context));

    if (FJ_FAILED) {
        return FJ_LAST_ERROR;
    }

    return FJ_OK;
}


static
void bus_close(
    void * bus_context
)
{
    fj_free(bus_context);
}


static
fj_err_t bus_serve(
    void * bus_context,
    fj_id_t serve_type,
    void *fjOPTION serve_data
)
{
    FJ_DEBUG_WARN_UNUSED(bus_context)
    FJ_DEBUG_WARN_UNUSED(serve_type)
    FJ_DEBUG_WARN_UNUSED(serve_data)

    return FJ_OK;
}


struct fj_bus const fj_wayland_bus = {
    .id = FJ_BUS_WAYLAND,
    .version = FJ_VERSION(0, 1),
    .open = bus_open,
    .close = bus_close,
    .serve = bus_serve,
};
