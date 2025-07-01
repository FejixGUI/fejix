#include <src/shared/utils/logging.h>
#include <src/shared/utils/memory.h>

#include <fejix/platform.h>

#include <stdlib.h>
#include <string.h>


// In alphabetic order
extern struct fj_platform fj_wayland_platform;
extern struct fj_platform fj_winapi_platform;
extern struct fj_platform fj_x11_platform;

// In alphabetic order
static struct fj_platform const *const platforms[] = {
#ifdef FJ_COMPILE_OPT_ENABLE_WAYLAND
    &fj_wayland_platform,
#endif
#ifdef FJ_COMPILE_OPT_ENABLE_WINAPI
    &fj_winapi_platform,
#endif
#ifdef FJ_COMPILE_OPT_ENABLE_X11
    &fj_x11_platform,
#endif
    NULL,  // Avoid warnings about the array being empty
};

// -1 accounts for the NULL at the end
static uint32_t const platforms_length = FJ_LEN(platforms) - 1;


void fj_platform_get_builtin_list(
    struct fj_platform const *const **out_platforms, uint32_t *out_platforms_length)
{
    *out_platforms = platforms;
    *out_platforms_length = platforms_length;
}


static struct fj_platform const *platform_find(char const *name)
{
    for (uint32_t i = 0; i < platforms_length; i++) {
        if (strcmp(platforms[i]->name, name) == 0) {
            return platforms[i];
        }
    }

    return NULL;
}


struct fj_platform const *fj_platform_load(void)
{
    struct fj_platform const *ret = NULL;
    char const *env;

    if (platforms_length == 0) {
        return NULL;
    }

    if (platforms_length == 1) {
        return platforms[0];
    }

    env = getenv("FEJIX_PLATFORM");
    if (env) {
        ret = platform_find(env);
        if (ret)
            return ret;
    }

#if defined(FJ_COMPILE_OPT_ENABLE_WAYLAND) || defined(FJ_COMPILE_OPT_ENABLE_X11)
    *env = getenv("XDG_SESSION_TYPE");
    if (env && (strcmp(env, "wayland") == 0 || strcmp(env, "x11") == 0)) {
        ret = platform_find(env);
        if (ret)
            return ret;
    }
#endif

#if defined(FJ_COMPILE_OPT_ENABLE_WAYLAND)
    if (getenv("WAYLAND_DISPLAY") != NULL) {
        ret = platform_find("wayland");
        if (ret)
            return ret;
    }
#endif

#if defined(FJ_COMPILE_OPT_ENABLE_X11)
    if (getenv("DISPLAY") != NULL) {
        ret = platform_find("x11");
        if (ret)
            return ret;
    }
#endif

    return platforms[0];
}
