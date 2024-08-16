#ifndef FEJIX_WAYLAND_CLIENT_H_
#define FEJIX_WAYLAND_CLIENT_H_


#include <fejix/interface/client.h>

#include <src/unixpoller/unixpoller.h>

#include <wayland-client.h>


struct fj_wayland_client {
    struct fj_client_callbacks const * callbacks;
    void * data;

    char const */*[]*/ name;

    struct fj_unixpoller unixpoller;
    fj_err_t callback_error;

    struct wl_display * display;
    struct wl_registry * registry;

    uint32_t compositor_id;
    uint32_t compositor_version;
    struct wl_compositor * compositor;
};


#endif
