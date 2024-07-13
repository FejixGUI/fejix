#ifndef FEJIX_WAYLAND_CLIENT_H_
#define FEJIX_WAYLAND_CLIENT_H_


#include <fejix/implementation.h>
#include <fejix/interface/client.h>

#include <src/shared/unixpoller/unixpoller.h>

#include <wayland-client.h>
#include <wayland-client-core.h>


struct fj_wayland_client_data {
    struct fj_client_callback const * callback;

    struct fj_unixpoller unixpoller;

    struct wl_display * display;
};


#endif
