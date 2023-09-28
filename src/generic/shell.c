#include <src/generic/shell.h>

#include <fejix/malloc.h>
#include <fejix/utils.h>


FJ_REQUIRE_VERSION(fj_shell, v_0_1)


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


fj_err_t fj_shell_window_init(
    struct fj_shell * shell,
    struct fj_client * client,
    struct fj_window * window
)
{
    return shell->window_init(client, window);
}


fj_err_t fj_shell_window_deinit(
    struct fj_shell * shell,
    struct fj_client * client,
    struct fj_window * window
)
{
    return shell->window_deinit(client, window);
}


struct fj_window * fj_window_new(const struct fj_window_listener * listener)
{
    if (listener == NULL) {
        return NULL;
    }

    struct fj_window * window = fj_alloc_zeroed(sizeof *window);

    if (window == NULL) {
        return NULL;
    }

    window->listener = listener;

    return window;
}


void fj_window_del(struct fj_window * window)
{
    fj_free(window);
}
