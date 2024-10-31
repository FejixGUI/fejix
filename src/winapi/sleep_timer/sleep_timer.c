#include <src/winapi/client/client.h>

#include <fejix/core/utils.h>


fj_timeout_t fj_winapi_sleep_timer_get_timeout(struct fj_client *client)
{
    return client->sleep_timer_manager.timeout;
}


static fj_err_t create_manager(
    struct fj_sleep_timer_manager **manager,
    struct fj_sleep_timer_manger_info *manager_info,
    struct fj_client *client
)
{
    *manager = &client->sleep_timer_manager;
    *manager_info = (struct fj_sleep_timer_manger_info) {
        .timeout_min = FJ_TIMEOUT_FROM_MILLIS(1),
        .timeout_max = FJ_TIMEOUT_FROM_MILLIS(INFINITE - 1),
    };

    return FJ_OK;
}


static fj_err_t destroy_manager(struct fj_sleep_timer_manager *manager)
{
    manager->timeout = 0;
    return FJ_OK;
}


static void set_timeout(struct fj_sleep_timer_manager *manager, fj_timeout_t timeout)
{
    manager->timeout = timeout;
}


static void unset_timeout(struct fj_sleep_timer_manager *manager)
{
    manager->timeout = 0;
}


struct fj_sleep_timer_funcs const fj_winapi_sleep_timer_funcs = {
    .create_manager = create_manager,
    .destroy_manager = destroy_manager,
    .set_timeout = set_timeout,
    .unset_timeout = unset_timeout,
};
