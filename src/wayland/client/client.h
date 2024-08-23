#ifndef FEJIX_WAYLAND_CLIENT_H_
#define FEJIX_WAYLAND_CLIENT_H_


#include <fejix/interface/client.h>
#include <fejix/interface/layer.h>

#include <src/unixpoller/unixpoller.h>

#include <fejix/core/vec.h>

#include <wayland-client.h>


struct fj_wayland_client {
    struct fj_client_callbacks const * callbacks;
    void * data;

    char const */*[]*/ name;

    struct fj_unixpoller unixpoller;

    struct wl_display * display;

    /** Wayland callbacks record their events to this vector for later handling.
        This vector must be preallocated on startup, and freed in case of allocation failure.
        To check if event recording succeeded, check if the vector has allocated. */
    struct fj_vec recorded_events;

    struct wl_registry * registry;

    uint32_t compositor_id;
    uint32_t compositor_version;
    struct wl_compositor * compositor;
    struct fj_layer_callbacks layer_callbacks;
};

struct fj_wayland_event_base {
    char const * wayland_interface_name;
    uint32_t wayland_proxy_id;
};

struct fj_wayland_event_wrapper {
    void const * event;

    fj_err_t (* handle)(struct fj_wayland_client * client, void const * event);
};


/** This handler is called once for every filtered event in a single pass. */
typedef fj_err_t (fj_wayland_event_handler_fn_t)(
    struct fj_wayland_client * client,
    void const * event
);

/** This filter is called once for every event in a single pass. */
typedef fj_bool32_t (fj_wayland_event_filter_fn_t)(
    struct fj_wayland_client * client,
    void * callback_data,
    void const * event
);



/** Allocates event_wrapper, event and initialises basic fields. */
fj_err_t fj_wayland_client_record_event(
    struct fj_wayland_client * client,
    struct fj_wayland_event_wrapper */*? out*/ * event_wrapper,
    fj_wayland_event_handler_fn_t * event_handler,
    size_t event_size,
    struct wl_proxy * event_proxy
);

void fj_wayland_client_record_fail(struct fj_wayland_client * client);


fj_err_t fj_wayland_client_handle_response(
    struct fj_wayland_client * client,
    void * callback_data,
    fj_wayland_event_filter_fn_t * event_filter
);


#endif
