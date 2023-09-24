#define FJ_INTERNAL_CLIENT_IMPLEMENTATION
#include "src/client.h"

#include <fejix/core/utils.h>
#include <fejix/core/malloc.h>

#include <stdlib.h>


static fj_utf8string_t get_direct_platform_choise(void) {
    fj_utf8string_t env = NULL;

    env = getenv("FEJIX_PLATFORM");

    if (env != NULL) {
        return env;
    }

    env = getenv("XDG_SESSION_TYPE");

    /* XDG_SESSION_TYPE is not documented, so we cannot be sure about what it
        contains */
    if (fj_streq(env, "x11") || fj_streq(env, "wayland")) {
        return env;
    }

    return NULL;
}


static fj_utf8string_t get_indirect_platform_guess(void)
{
    if (getenv("WINDIR") != NULL) {
        return "winapi";
    }

    if (getenv("WAYLAND_DISPLAY") != NULL) {
        return "wayland";
    }

    if (getenv("DISPLAY") != NULL) {
        return "x11";
    }

    return NULL;
}


static fj_utf8string_t get_default_platform(void) {
    if (FJ_ARRLEN(fj_platform_names) == 0) {
        return NULL;
    }

    return fj_platform_names[0];
}


static const struct fj_platform_runner * get_platform_runner(void)
{
    fj_utf8string_t platform_name = fj_get_selected_platform();

    for (uint32_t i = 0; i < FJ_ARRLEN(platform_runners); i++) {
        if (fj_streq(platform_runners[i].name, platform_name)) {
            return &platform_runners[i];
        }
    }

    return NULL;
}


fj_utf8string_t fj_get_selected_platform(void)
{
    fj_utf8string_t platform_name = NULL;

    platform_name = get_direct_platform_choise();

    if (platform_name != NULL) {
        return platform_name;
    }

    platform_name = get_indirect_platform_guess();

    if (platform_name != NULL) {
        return platform_name;
    }

    return get_default_platform();
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


void fj_client_set_schedule(struct fj_client * client, fj_schedule_t schedule)
{
    client->schedule = schedule;
}


fj_schedule_t fj_client_get_schedule(struct fj_client * client)
{
    return client->schedule;
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

