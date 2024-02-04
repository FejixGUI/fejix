#include <fejix/bus.h>
#include <fejix/utils.h>
#include <fejix/malloc.h>

#include "src/winapi/bus.h"


static
fj_err_t bus_open(
    void * FJ_NULLABLE FJ_OUT * bus_context,
    fj_bus_listener_t * bus_listener
)
{
    return FJ_OK;
}


static
void bus_close(
    void * bus_context
)
{

}


static
fj_err_t bus_serve(
    void * bus_context,
    fj_serve_type_t serve_type,
    void * FJ_NULLABLE serve_data
)
{
    return FJ_OK;
}


static
fj_err_t bus_commit(
    void * bus_context
)
{
    return FJ_OK;
}


struct fj_bus const fj_winapi_bus = {
    .id = FJ_BUS_WINAPI,
    .version = FJ_VERSION(0, 1),
    .open = bus_open,
    .close = bus_close,
    .serve = bus_serve,
    .commit = bus_commit,
};
