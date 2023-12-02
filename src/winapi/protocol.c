#include <fejix/protocol.h>
#include <fejix/utils.h>
#include <fejix/malloc.h>

#include "src/winapi/protocol.h"


static fj_err_t init_classes(struct fj_winapi_state * state)
{
    /* Reasons for the style flags:
        - `CS_OWNDC` - recommended for use with OpenGL. */

    WNDCLASSEX window_class = {
        .lpszClassName = FJ_WINAPI_WINDOW_CLASS_DEFAULT,
        .lpfnWndProc = NULL, // TODO
        .style = CS_OWNDC,
        .hCursor = LoadCursor(NULL, IDC_ARROW),
        .hInstance = state->instance,
        .cbSize = sizeof(WNDCLASSEX),
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hbrBackground = NULL,
        .lpszMenuName = NULL,
        .hIcon = NULL,
        .hIconSm = NULL,
    };

    return FJ_OK;
}


static fj_err_t init_state(struct fj_winapi_state * state)
{
    FJ_INIT_ERRORS

    state->instance = GetModuleHandle(NULL);

    FJ_TRY init_classes(state);

    if (FJ_FAILED) {
        return FJ_LAST_ERROR;
    }

    return FJ_OK;
}


static fj_err_t deinit_state(struct fj_winapi_state * state)
{
    return FJ_OK;
}


static fj_err_t invoke_main(struct fj_winapi_state * state)
{
    FJ_INIT_ERRORS

    FJ_TRY init_state(state);

    if (FJ_FAILED) {
        deinit_state(state);
        return FJ_LAST_ERROR;
    }

    // TODO mainloop

    FJ_TRY deinit_state(state);

    if (FJ_FAILED) {
        return FJ_LAST_ERROR;
    }

    return FJ_OK;
}


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
    FJ_INIT_ERRORS

    FJ_UNUSED(invoke_data)

    if (invoke_type != FJ_INVOKE_TYPE_MAIN) {
        return FJ_ERR("invalid invoke type");
    }

    struct fj_winapi_state * winapi_state = state;

    winapi_state->callback_data = callback_data;

    return invoke_main(winapi_state);
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
