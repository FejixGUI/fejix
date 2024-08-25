#ifndef FEJIX_WAYLAND_CLIENT_H_
#define FEJIX_WAYLAND_CLIENT_H_


#include <fejix/interface/client.h>
#include <fejix/interface/layer.h>

#include <src/unixpoller/unixpoller.h>

#include <fejix/core/vec.h>

#include <wayland-client.h>


/** Identifies only the global interfaces that are received on setup and not removed later.
    Even if such globals do get removed, nobody cares. */
typedef uint32_t fj_wayland_static_global_type_t;

enum fj_wayland_static_global_type {
    FJ_WAYLAND_STATIC_GLOBAL_COMPOSITOR,
    FJ_WAYLAND_STATIC_GLOBAL_SHM,

    FJ_WAYLAND_STATIC_GLOBAL_MAX,
};


typedef uint32_t fj_wayland_dynamic_global_type_t;

enum fj_wayland_dynamic_global_type {
    FJ_WAYLAND_DYNAMIC_GLOBAL_SEAT,
    FJ_WAYLAND_DYNAMIC_GLOBAL_OUTPUT,

    FJ_WAYLAND_DYNAMIC_GLOBAL_MAX,
};


/** Used to handle similar events of different interfaces in groups. */
typedef uint32_t fj_wayland_event_group_id_t;

enum fj_wayland_event_group_id {
    FJ_WAYLAND_EVENT_GROUP_GENERIC,

    /** An interface initialisation was requested. */
    FJ_WAYLAND_EVENT_GROUP_INTERFACE_INIT,
};


struct fj_wayland_global_desc {
    uint32_t id;
    uint32_t version;
};


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

    struct fj_wayland_global_desc static_globals[FJ_WAYLAND_STATIC_GLOBAL_MAX];
    struct fj_vec dynamic_globals[FJ_WAYLAND_DYNAMIC_GLOBAL_MAX];

    struct fj_wayland_layer_client_data */*?*/ layer_data;
};


struct fj_wayland_event_wrapper {
    void const * event;
    struct wl_proxy * event_source;
    fj_wayland_event_group_id_t event_group;

    fj_err_t (* handle)(
        struct fj_wayland_client * client,
        struct fj_wayland_event_wrapper const * event_wrapper
    );
};


/** Deep-copies the event wrapper and puts it onto the recorded list. */
fj_err_t fj_wayland_client_record_event(
    struct fj_wayland_client * client,
    struct fj_wayland_event_wrapper const * event_wrapper,
    size_t event_size
);

void fj_wayland_client_record_fail(struct fj_wayland_client * client);

fj_bool32_t fj_wayland_client_record_failed(struct fj_wayland_client * client);

/** Waits until all issued requests are processed. */
fj_err_t fj_wayland_client_roundtrip(struct fj_wayland_client * client);

/** This filter is called once for every event in a single pass. */
typedef fj_bool32_t (fj_wayland_event_filter_fn_t)(
    struct fj_wayland_client * client,
    void * callback_data,
    struct fj_wayland_event_wrapper const * event_wrapper
);

fj_err_t fj_wayland_client_handle_events(
    struct fj_wayland_client * client,
    void * filter_callback_data,
    fj_wayland_event_filter_fn_t * event_filter
);

/** Waits until all issued requests are processed and handles the filtered events,
    which may or may not be a response to a recently issued request. */
fj_err_t fj_wayland_client_roundtrip_and_handle_events(
    struct fj_wayland_client * client,
    void * filter_callback_data,
    fj_wayland_event_filter_fn_t * event_filter
);


#endif
