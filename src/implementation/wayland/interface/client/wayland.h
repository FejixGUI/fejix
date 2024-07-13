#ifndef FJ_WAYLAND_CLIENT_WAYLAND_H_
#define FJ_WAYLAND_CLIENT_WAYLAND_H_


#include <src/implementation/wayland/interface/client/client.h>


fj_err_t fj_wayland_init(struct fj_wayland_client_data * client);

fj_err_t fj_wayland_deinit(struct fj_wayland_client_data * client);


#endif
