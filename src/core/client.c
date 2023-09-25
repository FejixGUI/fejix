#define FJ_INTERNAL_CLIENT_IMPLEMENTATION
#include <src/core/client.h>

#include <fejix/utils.h>
#include <fejix/malloc.h>

#include <stdlib.h>


static const struct fj_platform_runner * get_platform_runner(void)
{
    fj_utf8string_t platform_name = fj_select_platform();

    for (uint32_t i = 0; i < FJ_ARRLEN(platform_runners); i++) {
        if (fj_str_eq(platform_runners[i].name, platform_name)) {
            return &platform_runners[i];
        }
    }

    return NULL;
}


fj_utf8string_t fj_select_platform(void)
{
    fj_utf8string_t env = getenv("FEJIX_PLATFORM");

    if (env != NULL) {
        return env;
    }

    env = getenv("XDG_SESSION_TYPE");

    /* XDG_SESSION_TYPE is not documented, so we cannot be sure about what it
        contains */
    if (fj_str_eq(env, "x11") || fj_str_eq(env, "wayland")) {
        return env;
    }

    if (FJ_ARRLEN(fj_platform_names) == 0) {
        return NULL;
    }

    return fj_platform_names[0];
}


void fj_client_get_platforms(uint32_t * count, fj_utf8string_t const ** names)
{
    if (count != NULL) {
        *count = FJ_ARRLEN(fj_platform_names);
    }

    if (names != NULL) {
        *names = fj_platform_names;
    }
}


struct fj_client * fj_client_new(fj_idstring_t client_id)
{
    struct fj_client * client = fj_alloc_zeroed(sizeof *client);

    if (client == NULL) {
        return NULL;
    }

    client->client_id = client_id;

    return client;
}


void fj_client_del(struct fj_client * client)
{
    fj_free(client);
}


fj_err_t fj_client_run(struct fj_client * client)
{
    if (client->client_listener == NULL) {
        return FJ_ERR("client listener is not set");
    }

    const struct fj_platform_runner * platform_runner = get_platform_runner();

    if (platform_runner == NULL) {
        return FJ_ERR("selected platform is not supported");
    }

    return platform_runner->run(client);
}


fj_ptr_t * fj_client_get_user_data(struct fj_client * client)
{
    return &client->user_data;
}


fj_schedule_t * fj_client_get_schedule(struct fj_client * client)
{
    return &client->schedule;
}


const struct fj_client_listener ** fj_client_get_listener(
    struct fj_client * client
)
{
    return &client->client_listener;
}


const struct fj_shell_listener ** fj_client_get_shell_listener(
    struct fj_client * client
)
{
#ifdef FJ_FEATURE_SHELL
    return &client->shell_listener;
#else
    (void) client;
    return NULL;
#endif
}

