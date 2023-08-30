#include "src/internal_prelude.h"

#include <stdlib.h>


struct platform_runner {
    fj_utf8string_t name;
    fj_err_t (*run)(struct fj_client *, fj_idstring_t client_name);
};


static const struct platform_runner platform_runners[] = {
#ifdef FJ_PLATFORM_X11
    { "x11", fj_x11_client_run },
#endif
};


static fj_utf8string_t guess_default_platform(void)
{
#if defined(FJ_PLATFORM_WINAPI)
    return "winapi";
#elif defined(FJ_PLATFORM_WAYLAND)
    return "wayland";
#elif defined(FJ_PLATFORM_X11)
    return "x11";
#else
#   error Cannot select default platform. Did you specify/implement it? \
(note: specify the platform with FEJIX_PLATFORM_XXX=ON)
#endif
}


static fj_utf8string_t guess_platform(void)
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

    return guess_default_platform();
}


static fj_utf8string_t get_platform_name(void)
{
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

    return guess_platform();
}


static const struct platform_runner * get_platform_runner(
    fj_utf8string_t platform_name
)
{
    for (uint32_t i = 0; i < FJ_ARRLEN(platform_runners); i++) {
        if (fj_streq(platform_runners[i].name, platform_name)) {
            return &platform_runners[i];
        }
    }

    return NULL;
}


struct fj_client * fj_client_new(void)
{
    return fj_alloc_zeroed(sizeof(struct fj_client));
}


void fj_client_del(struct fj_client * client)
{
    fj_free(client);
}


fj_ptr_t * fj_client_get_user_data(struct fj_client * client)
{
    return &client->user_data;
}


const struct fj_client_listener ** fj_client_get_listener(
    struct fj_client * client
)
{
    return &client->client_listener;
}


fj_err_t fj_client_run(
    struct fj_client * client,
    fj_idstring_t client_name
)
{
    if (client->client_listener == NULL) {
        return FJ_ERR("client listener is not set");
    }

    fj_utf8string_t platform_name = get_platform_name();

    if (platform_name == NULL) {
        return FJ_ERR("No platform selected. "
            "Define any of the environment variables:"
            "FEJIX_PLATFORM, XDG_SESSION_TYPE, DISPLAY, WAYLAND_DISPLAY"
        );
    }

    const struct platform_runner * runner;
    runner = get_platform_runner(platform_name);

    if (runner == NULL) {
        return FJ_ERR("selected platform is not supported");
    }

    return runner->run(client, client_name);
}


const struct fj_unixpoller_listener ** fj_client_get_unixpoller_listener(
    struct fj_client * client
)
{
#ifdef FJ_FEATURE_UNIXPOLLER
    return &client->unixpoller_listener;
#else
    (void) client;
    return NULL;
#endif
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
