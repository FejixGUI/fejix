#include <fejix/protocol.h>
#include <fejix/utils.h>
#include <fejix/malloc.h>

#include "src/winapi/protocol.h"


static fj_err_t create_state(void * FJ_NULLABLE FJ_OUT * state)
{
    return fj_alloc_zeroed(state, sizeof(struct fj_winapi_state));
}

void (* destroy_state)(
    void * state
);

void (* set_callback_data)(
    void * state,
    void * FJ_NULLABLE callback_data
);

fj_err_t (* run)(
    void * state,
    fj_invoke_type_t invoke_type,
    void * FJ_NULLABLE invoke_data
);


struct fj_protocol const fj_winapi_protocol = {
    .name = FJ_UTF8("winapi"),
    .version = FJ_VERSION(0, 1),

    .interface_count = 0,
    .interfaces = NULL,


};
