#include <fejix/bus.h>
#include <fejix/utils.h>

#include <stdio.h>


static
fj_err_t bus_open(
    void * FJ_NULLABLE FJ_OUT * bus_context
)
{
    FJ_UNUSED(bus_context)
    FJ_UNUSED(bus_listener)
    return FJ_OK;
}


static
void bus_close(
    void * bus_context
)
{
    FJ_UNUSED(bus_context)
}


static
fj_err_t bus_serve(
    void * bus_context,
    fj_id_t serve_type,
    void * FJ_NULLABLE serve_data
)
{
    FJ_UNUSED(bus_context)
    FJ_UNUSED(serve_type)
    FJ_UNUSED(serve_data)

    printf(
        "Note: Fejix is running a NOOP bus, this is just a test."
    );

    return FJ_OK;
}


static
fj_err_t bus_commit(
    void * bus_context
)
{
    FJ_UNUSED(bus_context)
    return FJ_OK;
}


struct fj_bus const fj_noop_bus = {
    .id = FJ_BUS_NOOP,
    .version = FJ_VERSION(0, 1),
    .open = bus_open,
    .close = bus_close,
    .serve = bus_serve,
};
