#ifndef FEJIX_WAYLAND_CLIENT_H_
#define FEJIX_WAYLAND_CLIENT_H_


#include <fejix/interface/client.h>
#include <fejix/interface/presentation.h>

#include <src/unixpoller/unixpoller.h>

#include <fejix/core/vec.h>

#include <wayland-client.h>


typedef uint32_t fj_wayland_interface_id_t;

enum fj_wayland_interface_id {
    FJ_WAYLAND_INTERFACE_COMPOSITOR,
    FJ_WAYLAND_INTERFACE_SHM,
    FJ_WAYLAND_INTERFACE_SEAT,
    FJ_WAYLAND_INTERFACE_OUTPUT,

    FJ_WAYLAND_INTERFACE_MAX,
};


typedef uint32_t fj_wayland_interface_type_t;

enum fj_wayland_interface_type {
    /** A singleton global object of such interface is advertised at startup.
        Nobody cares if such global object gets removed leter. */
    FJ_WAYLAND_INTERFACE_STATIC,

    /** A global object of such interface may be dynamically added or removed during the client
        lifetime. */
    FJ_WAYLAND_INTERFACE_DYNAMIC,
};


/** Used to handle similar events of different interfaces in groups. */
typedef uint32_t fj_wayland_event_group_id_t;

enum fj_wayland_event_group_id {
    FJ_WAYLAND_EVENT_GROUP_GENERIC,

    /** An interface initialisation was requested. */
    FJ_WAYLAND_EVENT_GROUP_INTERFACE_INIT,
};


struct fj_wayland_global {
    uint32_t id;
    uint32_t version;
};

union fj_wayland_interface_desc {
    /** Static singleton object. */
    struct fj_wayland_global global;

    /** List of dynamic globals of the same interface. */
    struct fj_vec global_list;
};


struct fj_wayland_client {
    struct fj_client_callbacks const * callbacks;

    /** User data */
    void */*?*/ data;

    char const */*[]*/ name;

    struct wl_display * display;
    struct wl_registry * registry;
    union fj_wayland_interface_desc interfaces[FJ_WAYLAND_INTERFACE_MAX];

    struct fj_unixpoller unixpoller;

    /** Wayland callbacks record their events to this vector for later handling.
        This vector must be preallocated on startup, and freed in case of allocation failure.
        To check if event recording succeeded, check if the vector has allocated. */
    struct fj_vec recorded_events;

    struct fj_wayland_presentation_global_data */*?*/ presentation;

#ifdef FJ_OPT_FEATURE_SOFTER_CANVAS
    struct fj_wayland_softer_canvas_global_data */*?*/ softer;
#endif
};


struct fj_wayland_event_wrapper {
    void const */*?*/ event;

    /** Used to copy the event. If event is NULL, set to 0. */
    size_t event_size;

    struct wl_proxy * event_source;

    fj_wayland_event_group_id_t event_group;

    fj_err_t (* handle)(
        struct fj_wayland_client * client,
        struct fj_wayland_event_wrapper const * event_wrapper
    );
};


fj_wayland_interface_type_t fj_wayland_get_interface_type(fj_wayland_interface_id_t interface_id);

/** interface_id returns FJ_WAYLAND_INTERFACE_MAX on failure. */
void fj_wayland_get_global_by_id(
    struct fj_wayland_client * client,
    uint32_t object_id,
    fj_wayland_interface_id_t /*out*/ * interface_id,
    struct fj_wayland_global const */*? out*/ * global
);

/** Returns NULL if the global does not exist. */
struct fj_wayland_global const */*?*/ fj_wayland_get_static_global(
    struct fj_wayland_client * client,
    fj_wayland_interface_id_t interface_id
);

fj_err_t fj_wayland_bind_global(
    struct fj_wayland_client * client,
    fj_wayland_interface_id_t interface_id,
    struct fj_wayland_global const * global,
    void */*? out*/ * object
);


/** Deep-copies the event wrapper and puts it onto the recorded list.

    This is the only fallible function allowed in Wayland callbacks. */
fj_err_t fj_wayland_record_event(
    struct fj_wayland_client * client,
    struct fj_wayland_event_wrapper const * event_wrapper
);

/** This must be called in case of failure inside Wayland callbacks.

    This should only be called in Wayland callbacks and not in other functions. */
void fj_wayland_record_fail(struct fj_wayland_client * client);

fj_bool32_t fj_wayland_record_failed(struct fj_wayland_client * client);

/** Waits until all issued requests are processed. */
fj_err_t fj_wayland_roundtrip(struct fj_wayland_client * client);

/** This filter is called once for every event in a single pass. */
typedef fj_bool32_t (fj_wayland_event_filter_fn_t)(
    struct fj_wayland_client * client,
    void */*?*/ callback_data,
    struct fj_wayland_event_wrapper const * event_wrapper
);

fj_err_t fj_wayland_handle_events(
    struct fj_wayland_client * client,
    void */*?*/ filter_callback_data,
    fj_wayland_event_filter_fn_t * event_filter
);


#endif
