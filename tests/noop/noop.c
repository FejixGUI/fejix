#include <fejix/bus.h>
#include <fejix/utils.h>
#include <fejix/ext.h>


fj_err_t listener(void * bus_context, const struct fj_message * msg)
{
    FJ_INTEND_UNUSED(bus_context);
    FJ_INTEND_UNUSED(msg);

    return FJ_OK;
}


int main(void) {
    return fj_ext_standalone_main(listener);
}
