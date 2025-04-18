#ifndef FEJIX_WAYLAND_CLIENT_INCLUDED
#define FEJIX_WAYLAND_CLIENT_INCLUDED


#include <src/unixpoller/unixpoller.h>

#include <fejix/interface/client.h>
#include <fejix/interface/output.h>

#include <fejix/core/vec.h>

#include <wayland-client.h>


typedef uint32_t fj_wayland_interface_id;

enum {
    FJ_WAYLAND_INTERFACE_COMPOSITOR,
    FJ_WAYLAND_INTERFACE_SHM,
    FJ_WAYLAND_INTERFACE_SEAT,
    FJ_WAYLAND_INTERFACE_OUTPUT,

    FJ_WAYLAND_INTERFACE_MAX,
};


struct fj_wayland_global {
    uint32_t id;
    uint32_t version;
};

union fj_wayland_interface_desc {
    /** Singleton object. */
    struct fj_wayland_global global;

    /** List of dynamic globals of the same interface. */
    struct fj_vec global_list;
};


struct fj_client {
    union fj_tag tag;

    struct fj_client_callbacks callbacks;

    char const * /*[]*/ name;

    struct wl_display *display;
    struct wl_registry *registry;
    union fj_wayland_interface_desc interfaces[FJ_WAYLAND_INTERFACE_MAX];
    enum fj_error callback_error;

    struct fj_unixpoller unixpoller;

    struct fj_wayland_output_global_data * /*?*/ output;

#ifdef FJ_OPT_FEATURE_SOFTER_CANVAS
    struct fj_wayland_softer_canvas_global_data * /*?*/ softer;
#endif
};


fj_bool8 fj_wayland_interface_is_singleton(fj_wayland_interface_id interface_id);

/** interface_id returns FJ_WAYLAND_INTERFACE_MAX on failure. */
void fj_wayland_get_global_by_id(
    struct fj_client *client,
    uint32_t object_id,
    fj_wayland_interface_id /*out*/ *interface_id,
    struct fj_wayland_global const * /*? out*/ *global);

/** Returns NULL if the global does not exist. */
struct fj_wayland_global const * /*?*/ fj_wayland_get_static_global(
    struct fj_client *client, fj_wayland_interface_id interface_id);

enum fj_error fj_wayland_bind_global(
    struct fj_client *client,
    fj_wayland_interface_id interface_id,
    struct fj_wayland_global const *global,
    void * /*? out*/ *object);

/** Waits until all issued requests are processed and dispatches the queue. */
enum fj_error fj_wayland_roundtrip(struct fj_client *client);

/** Waits for the next event and dispatches the queue. */
enum fj_error fj_wayland_wait_for_events(struct fj_client *client);

enum fj_error fj_wayland_handle_events(
    struct fj_client *client,
    void * /*?*/ filter_callback_data,
    fj_wayland_event_filter_fn *event_filter);


#endif
