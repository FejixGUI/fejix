#include <fejix/implementation.h>
#include <fejix/utils.h>
#include <fejix/malloc.h>


void const *fjOPTION noop_get_interface(
    struct fj_implementation const * implementation,
    fj_enum32_t interface_id
)
{
    (void) implementation;
    (void) interface_id;

    return NULL;
}



struct fj_implementation fj_noop_implementation = {
    .implementation_id = FJ_IMPLEMENTATION_NOOP,
    .static_data = NULL,
    .get_interface = noop_get_interface,
};
