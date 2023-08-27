#include "src/internal_prelude.h"

#include <stdlib.h>


#define NO_PLATFORM_ERROR "No platform selected. " \
    "Define FEJIX_PLATFORM or XDG_SESSION_TYPE or DISPLAY or WAYLAND_DISPLAY"


struct platform {
    fj_utf8string_t name;
    fj_err_t (*run)(struct fj_client *);
};


static const struct platform platforms[] = {
#ifdef FJ_PLATFORM_X11
    { "x11", fj_x11_run },
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


static fj_utf8string_t get_platform(void)
{
    fj_utf8string_t env = NULL;

    env = getenv("FEJIX_PLATFORM");
    
    if (env != NULL) {
        return env;
    }

    env = getenv("XDG_SESSION_TYPE");
    
    /* XDG_SESSION_TYPE is not documented, so we cannot be sure about what it
        contains */
    if (FJ_STREQ(env, "x11") || FJ_STREQ(env, "wayland")) {
        return env;
    }

    return guess_platform();
}


/* Run the platform which matches the name. */
static fj_err_t run_platform(
    struct fj_client * client,
    fj_utf8string_t platform_name
)
{
    for (uint32_t i = 0; i < FJ_ARRLEN(platforms); i++) {
        if (FJ_STREQ(platforms[i].name, platform_name)) {
            return platforms[i].run(client);
        }
    }

    return FJ_ERR("selected platform does not exist");
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


fj_err_t fj_client_run(struct fj_client * client)
{

    fj_utf8string_t platform_name = get_platform();

    if (platform_name == NULL) {
        return FJ_ERR(NO_PLATFORM_ERROR);
    }

    return run_platform(client, platform_name);
}
