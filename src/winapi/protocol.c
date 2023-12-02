#include <fejix/protocol.h>
#include <fejix/utils.h>
#include <fejix/malloc.h>

#include "src/winapi/protocol.h"


static fj_err_t create_state(void * FJ_NULLABLE FJ_OUT * state)
{
    return fj_alloc_zeroed(state, sizeof(struct fj_winapi_state));
}

static void destroy_state(void * state)
{
    fj_free(state);
}

static fj_err_t invoke(
    void * state,
    void * FJ_NULLABLE callback_data,
    fj_invoke_type_t invoke_type,
    void * FJ_NULLABLE invoke_data
)
{
    return FJ_OK;
}

fj_err_t execute_commands(
    void * state,
    uint32_t command_count,
    struct fj_command const * commands,
    fj_bool_t * executed_flags
)
{
    return FJ_OK;
}


struct fj_protocol const fj_winapi_protocol = {
    .name = FJ_UTF8("winapi"),
    .version = FJ_VERSION(0, 1),

    .interface_count = 0,
    .interfaces = NULL,

    .create_state = create_state,
    .destroy_state = destroy_state,
    .invoke = invoke,
    .execute_commands = execute_commands,
};
