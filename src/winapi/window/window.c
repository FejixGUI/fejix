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


static fj_err_t create_window(
    struct fj_window_manager *manager,
    struct fj_window **window,
    struct fj_window_create_info const *create_info
)
{
    struct fj_window window_value = {
        .tag = create_info->tag,
        .manager = manager,
    };

    FJ_TRY (FJ_ALLOC_COPIED(window, &window_value)) {
        return fj_result;
    }

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


struct fj_window_funcs fj_winapi_window_funcs = {
    .create_manager = create_manager,
    .destroy_manager = destroy_manager,
    .create_window = create_window,
    .destroy_window = destroy_window,
    .update_windows = update_windows,
};
