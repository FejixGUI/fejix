#include <fejix/platform.h>

#include <src/shared/common/macros.h>

#include <stdlib.h>
#include <string.h>


extern struct fj_platform fj_wl_platform;
extern struct fj_platform fj_winapi_platform;
extern struct fj_platform fj_x11_platform;

// In alphabetic order
static struct fj_platform const *const platforms[] = {
#ifdef FJ_OPT_WAYLAND
    &fj_wl_platform,
#endif
#ifdef FJ_OPT_WINAPI
    &fj_winapi_platform,
#endif
#ifdef FJ_OPT_X11
    &fj_x11_platform,
#endif
    NULL,  // Avoid warnings about the array being empty
};

enum
{
    // -1 accounts for the NULL at the end
    PLATFORMS_LENGTH = FJ_LEN(platforms) - 1
};


void fj_platform_get_builtin_list(
    struct fj_platform const *const **out_platforms,
    uint32_t *out_platforms_length)
{
    *out_platforms = platforms;
    *out_platforms_length = PLATFORMS_LENGTH;
}


static struct fj_platform const *platform_find(char const *name)
{
    for (uint32_t i = 0; i < PLATFORMS_LENGTH; i++) {
        if (strcmp(platforms[i]->name, name) == 0) {
            return platforms[i];
        }
    }

    return NULL;
}


struct fj_platform const *fj_platform_load(void)
{
    struct fj_platform const *platform = NULL;
    char const *env;

    if (PLATFORMS_LENGTH == 0) {
        return NULL;
    }

    if (PLATFORMS_LENGTH == 1) {
        return platforms[0];
    }

    env = getenv("FEJIX_PLATFORM");
    if (env) {
        platform = platform_find(env);
        if (platform)
            return platform;
    }

#if defined(FJ_OPT_WAYLAND) || defined(FJ_OPT_X11)
    env = getenv("XDG_SESSION_TYPE");
    if (env && (strcmp(env, "wayland") == 0 || strcmp(env, "x11") == 0)) {
        platform = platform_find(env);
        if (platform)
            return platform;
    }
#endif

#if defined(FJ_OPT_WAYLAND)
    if (getenv("WAYLAND_DISPLAY") != NULL) {
        platform = platform_find("wayland");
        if (platform)
            return platform;
    }
#endif

#if defined(FJ_OPT_X11)
    if (getenv("DISPLAY") != NULL) {
        platform = platform_find("x11");
        if (platform)
            return platform;
    }
#endif

    return platforms[0];
}
