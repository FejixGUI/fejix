#ifndef FEJIX_INTERFACE_SHELL_H_
#define FEJIX_INTERFACE_SHELL_H_


#include <fejix/base.h>


struct fj_client;

struct fj_shell {
    int _todo;
};

struct fj_shell_listener {
    void (*init)(
        struct fj_client * client,
        struct fj_shell * shell
    );
};


#endif
