#ifndef FEJIX_INTERFACE_SHELL_H_
#define FEJIX_INTERFACE_SHELL_H_


#include <fejix/client.h>


struct fj_window;

FJ_DEFINE_VERSION(fj_shell, v_0_1)
struct fj_shell {
    fj_err_t (*window_init)(
        struct fj_client * client,
        struct fj_window * window
    );

    fj_err_t (*window_deinit)(
        struct fj_client * client,
        struct fj_window * window
    );
};

FJ_DEFINE_VERSION(fj_shell_listener, v_0_1)
struct fj_shell_listener {
    void (*init)(
        struct fj_client * client,
        struct fj_shell * shell
    );
};

FJ_DEFINE_VERSION(fj_window_listener, v_0_1)
struct fj_window_listener {
    fj_err_t (*init)(
        struct fj_client * client,
        struct fj_window * window
    );

    fj_err_t (*deinit)(
        struct fj_client * client,
        struct fj_window * window
    );
};


const struct fj_shell_listener ** fj_client_get_shell_listener(
    struct fj_client * client
);

struct fj_window * fj_window_new(const struct fj_window_listener * listener);

void fj_window_del(struct fj_window * window);


#endif
