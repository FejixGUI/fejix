#include <fejix/platform.h>

#include <src/shared/common/macros.h>


fj_generic_dispatcher fj_x11_app_dispatcher, fj_x11_window_service_dispatcher,
    fj_x11_window_dispatcher;

static fj_generic_dispatcher const *const dispatchers[] = {
    [FJ_TYPE_APP] = &fj_x11_app_dispatcher,
    [FJ_TYPE_WINDOW_SERVICE] = &fj_x11_window_service_dispatcher,
    [FJ_TYPE_WINDOW] = &fj_x11_window_dispatcher,
};

static fj_generic_dispatcher get_dispatcher(enum fj_type type)
{
    if (type >= FJ_LEN(dispatchers) || dispatchers[type] == NULL)
        return NULL;

    return *dispatchers[type];
}

struct fj_platform fj_x11_platform = {
    .name = "x11",
    .version = { FJ_VERSION_MAJOR, FJ_VERSION_MINOR, FJ_VERSION_PATCH },
    .get_dispatcher = get_dispatcher,
};
