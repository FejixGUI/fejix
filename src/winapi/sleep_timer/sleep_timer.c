#include <src/winapi/client/client.h>

#include <fejix/core/utils.h>


fj_timeout_t fj_winapi_sleep_timer_get_timeout(struct fj_client *client)
{
    return client->sleep_timer_impl.timeout;
}


static fj_err_t create_impl(
    struct fj_client *client,
    struct fj_sleep_timer_impl **impl,
    struct fj_sleep_timer_caps *caps
)
{
    *impl = &client->sleep_timer_impl;
    *caps = (struct fj_sleep_timer_caps) {
        .timeout_min = FJ_TIMEOUT_FROM_MILLIS(1),
        .timeout_max = FJ_TIMEOUT_FROM_MILLIS(INFINITE - 1),
    };

    return FJ_OK;
}


static fj_err_t destroy_impl(struct fj_client *client, struct fj_sleep_timer_impl *impl)
{
    (void) client;
    impl->timeout = 0;
    return FJ_OK;
}


static void set_timeout(
    struct fj_client *client,
    struct fj_sleep_timer_impl *impl,
    fj_timeout_t timeout
)
{
    (void) client;
    impl->timeout = timeout;
}

static void unset_timeout(struct fj_client *client, struct fj_sleep_timer_impl *impl)
{
    (void) client;
    impl->timeout = 0;
}


struct fj_sleep_timer_iface const fj_winapi_sleep_timer_iface = {
    .create_impl = create_impl,
    .destroy_impl = destroy_impl,
    .set_timeout = set_timeout,
    .unset_timeout = unset_timeout,
};
