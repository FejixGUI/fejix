#include <src/winapi/window/window.h>

#include <fejix/core/alloc.h>
#include <fejix/core/utils.h>


static fj_err_t create_manager(
    struct fj_window_manager **manager,
    struct fj_client *client,
    struct fj_window_callbacks const *callbacks
)
{
    FJ_TRY (FJ_ALLOC_ZEROED(manager)) {
        return fj_result;
    }

    (*manager)->client = client;

    return FJ_OK;
}


static fj_err_t destroy_manager(struct fj_window_manager *manager)
{
    FJ_FREE(&manager);

    return FJ_OK;
}


static fj_err_t create_window_builder(
    struct fj_window_manager *manager,
    struct fj_window_builder **window_builder,
    struct fj_window_create_info const *window_info
)
{
    return FJ_OK;
}

static fj_err_t destroy_window_builder(
    struct fj_window_manager *manager,
    struct fj_window_builder *window_builder
)
{
    return FJ_OK;
}


static fj_err_t create_window(
    struct fj_window_manager *manager,
    struct fj_window **window,
    struct fj_window_builder *window_builder
)
{
    FJ_TRY (FJ_ALLOC_ZEROED(window)) {
        return fj_result;
    }

    **window = (struct fj_window) {
        .tag = window_builder->window_tag,
        .manager = manager,
        .class_atom = window_builder->window_class_atom,
    };

    return FJ_OK;
}


static fj_err_t destroy_window(struct fj_window_manager *manager, struct fj_window *window)
{
    if (window->handle != NULL) {
        DestroyWindow(window->handle);
    }

    FJ_FREE(&window);

    return FJ_OK;
}


static fj_err_t update_windows(
    struct fj_window_manager *manager,
    struct fj_window *const *windows,
    uint32_t window_count
)
{
    return FJ_OK;
}


struct fj_window_interface fj_winapi_window_interface = {
    .create_manager = create_manager,
    .destroy_manager = destroy_manager,
    .create_window_builder = create_window_builder,
    .destroy_window_builder = destroy_window_builder,
    .create_window = create_window,
    .destroy_window = destroy_window,
    .update_windows = update_windows,
};
