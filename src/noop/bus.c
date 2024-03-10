#include <fejix/bus.h>
#include <fejix/utils.h>
#include <fejix/malloc.h>

#include <stdio.h>


struct fj_noop_bus_context {
    struct fj_bus_context_base FJ_INHERIT base;
};


static
fj_err_t bus_open(
    void * FJ_NULLABLE FJ_OUT * bus_context
)
{
    return fj_alloc_zeroed(bus_context, sizeof(struct fj_noop_bus_context));
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
    void * FJ_NULLABLE serve_data
)
{
    FJ_INTEND_UNUSED(bus_context)
    FJ_INTEND_UNUSED(serve_type)
    FJ_INTEND_UNUSED(serve_data)

    printf("Note: Fejix is running NOOP bus, it does nothing.\n");

    return FJ_OK;
}


struct fj_bus const fj_noop_bus = {
    .id = FJ_BUS_NOOP,
    .version = FJ_VERSION(0, 1),
    .open = bus_open,
    .close = bus_close,
    .serve = bus_serve,
};
