#ifndef FJ_GENERIC_SHELL_H_
#define FJ_GENERIC_SHELL_H_


#include <src/generic/client.h>

#include <fejix/interface/shell.h>


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


struct fj_window {
    const struct fj_window_listener * listener;
};


#endif
