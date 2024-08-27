#include <src/wayland/client/client.h>

#include <fejix/core/alloc.h>
#include <fejix/core/utils.h>

#include <wayland-client-core.h>
#include <wayland-client-protocol.h>

#include <string.h>
#include <errno.h>


struct wayland_dynamic_global_event {
    struct fj_wayland_global global;
    fj_wayland_interface_id_t interface_id;
    fj_bool32_t added;
};


static
char const */*[]?*/ wayland_interface_names[] = {
    [FJ_WAYLAND_INTERFACE_MAX] = "...static global max ID...",

    [FJ_WAYLAND_INTERFACE_COMPOSITOR] = "wl_compositor",
    [FJ_WAYLAND_INTERFACE_SHM] = "wl_shm",
    [FJ_WAYLAND_INTERFACE_SEAT] = "wl_seat",
    [FJ_WAYLAND_INTERFACE_OUTPUT] = "wl_output",
};

static
fj_wayland_interface_type_t wayland_interface_types[] = {
    [FJ_WAYLAND_INTERFACE_MAX] = FJ_WAYLAND_INTERFACE_STATIC,

    [FJ_WAYLAND_INTERFACE_COMPOSITOR] = FJ_WAYLAND_INTERFACE_STATIC,
    [FJ_WAYLAND_INTERFACE_SHM] = FJ_WAYLAND_INTERFACE_STATIC,
    [FJ_WAYLAND_INTERFACE_SEAT] = FJ_WAYLAND_INTERFACE_DYNAMIC,
    [FJ_WAYLAND_INTERFACE_OUTPUT] = FJ_WAYLAND_INTERFACE_DYNAMIC,
};


static
struct wl_interface const * wayland_interfaces[] = {
    [FJ_WAYLAND_INTERFACE_MAX] = NULL,

    [FJ_WAYLAND_INTERFACE_COMPOSITOR] = &wl_compositor_interface,
    [FJ_WAYLAND_INTERFACE_SHM] = &wl_shm_interface,
    [FJ_WAYLAND_INTERFACE_SEAT] = &wl_seat_interface,
    [FJ_WAYLAND_INTERFACE_OUTPUT] = &wl_output_interface,
};



#ifdef FJ_OPT_FEATURE_SOFTER_CANVAS
fj_wayland_interface_cleanup_fn_t fj_wayland_softer_canvas_cleanup;
#endif
fj_wayland_interface_cleanup_fn_t fj_wayland_layer_cleanup;

static
fj_wayland_interface_cleanup_fn_t * cleanup_functions[] = {
#ifdef FJ_OPT_FEATURE_SOFTER_CANVAS
    fj_wayland_softer_canvas_cleanup,
#endif
    fj_wayland_layer_cleanup,
};


/** Returns FJ_WAYLAND_INTERFACE_MAX if the global is not found in the list. */
static
uint32_t wayland_get_interface_id(char const */*[]*/ interface_name)
{
    for (uint32_t i=0; i<FJ_ARRAY_LEN(wayland_interface_names); i++) {
        if (wayland_interface_names[i] == NULL) {
            continue;
        }

        if (fj_streq(wayland_interface_names[i], interface_name)) {
            return i;
        }
    }

    return FJ_WAYLAND_INTERFACE_MAX;
}


fj_wayland_interface_type_t fj_wayland_get_interface_type(fj_wayland_interface_id_t interface_id)
{
    return wayland_interface_types[interface_id];
}


struct wl_interface const * wayland_get_interface(fj_wayland_interface_id_t interface_id)
{
    return wayland_interfaces[interface_id];
}


void fj_wayland_get_global(
    struct fj_wayland_client * client,
    uint32_t object_id,
    fj_wayland_interface_id_t /*out*/ * out_interface_id,
    struct fj_wayland_global const */*? out*/ * out_global
)
{
    *out_interface_id = FJ_WAYLAND_INTERFACE_MAX;
    *out_global = NULL;

    for (uint32_t i=0; i<FJ_ARRAY_LEN(client->interfaces); i++) {
        switch (fj_wayland_get_interface_type(i)) {
            case FJ_WAYLAND_INTERFACE_STATIC:
                if (client->interfaces[i].global.id == object_id) {
                    *out_interface_id = i;
                    *out_global = &client->interfaces[i].global;
                }
            break;

            case FJ_WAYLAND_INTERFACE_DYNAMIC:
                for (uint32_t j=0; j<client->interfaces[i].global_list.length; j++) {
                    struct fj_wayland_global const * global =
                        fj_vec_offset(&client->interfaces[i].global_list, j);

                    if (global->id == object_id) {
                        *out_interface_id = i;
                        *out_global = global;
                    }
                }
            break;
        }
    }
}


fj_bool32_t fj_wayland_get_static_global(
    struct fj_wayland_client * client,
    fj_wayland_interface_id_t interface_id,
    struct fj_wayland_global const */*? out*/ * global
)
{
    *global = &client->interfaces[interface_id].global;

    return (*global)->id != 0;
}


static
fj_err_t wayland_handle_dynamic_global_event(
    struct fj_wayland_client * client,
    struct fj_wayland_event_wrapper const * event_wrapper
)
{
    FJ_INIT_TRY

    struct wayland_dynamic_global_event const * event = event_wrapper->event;

    struct fj_vec * global_list = &client->interfaces[event->interface_id].global_list;

    if (event->added) {
        FJ_TRY(fj_vec_push(global_list, &event->global)) {
            return FJ_RESULT;
        }
    }

    // TODO Delegate Wayland dynamic global events to specialised interfaces (if initialised).

    if (!event->added) {
        for (uint32_t i=0; i<global_list->length; i++) {
            struct fj_wayland_global const * global = fj_vec_offset(global_list, i);
            if (global->id == event->global.id) {
                FJ_TRY(fj_vec_remove(global_list, i, 1)) {
                    return FJ_RESULT;
                }
            }
        }
    }

    return FJ_OK;
}


static
void wayland_handle_static_global(
    struct fj_wayland_client * client,
    fj_wayland_interface_id_t interface_id,
    struct fj_wayland_global const * global
)
{
    client->interfaces[interface_id].global = *global;
}


static
void wayland_handle_dynamic_global(
    struct fj_wayland_client * client,
    fj_wayland_interface_id_t interface_id,
    struct fj_wayland_global const * global,
    fj_bool32_t added
)
{
    FJ_INIT_TRY

    struct wayland_dynamic_global_event const event = {
        .global = *global,
        .interface_id = interface_id,
        .added = added,
    };

    struct fj_wayland_event_wrapper const wrapper = {
        .event = &event,
        .event_size = sizeof(event),
        .event_group = FJ_WAYLAND_EVENT_GROUP_GENERIC,
        .event_source = (struct wl_proxy *) client->registry,
        .handle = wayland_handle_dynamic_global_event,
    };

    FJ_TRY(fj_wayland_record_event(client, &wrapper)) {
        fj_wayland_record_fail(client);
        return;
    }
}


static
void wayland_registry_add(
    void * client_,
    struct wl_registry * _registry,
    uint32_t object_id,
    char const * interface_name,
    uint32_t interface_version
)
{
    FJ_ARG_FROM_OPAQUE(client, struct fj_wayland_client *)
    FJ_ARG_UNUSED(registry)

    struct fj_wayland_global const global = {
        .id = object_id,
        .version = interface_version,
    };

    fj_wayland_interface_id_t interface_id = wayland_get_interface_id(interface_name);

    if (interface_id == FJ_WAYLAND_INTERFACE_MAX) {
        return;
    }

    switch (fj_wayland_get_interface_type(interface_id)) {
        case FJ_WAYLAND_INTERFACE_STATIC:
            wayland_handle_static_global(client, interface_id, &global);
        return;

        case FJ_WAYLAND_INTERFACE_DYNAMIC:
            wayland_handle_dynamic_global(client, interface_id, &global, true);
        return;
    }
}


static
void wayland_registry_remove(void * client_, struct wl_registry * _registry, uint32_t object_id)
{
    FJ_ARG_FROM_OPAQUE(client, struct fj_wayland_client *)
    FJ_ARG_UNUSED(registry)

    uint32_t interface_id;
    struct fj_wayland_global const * global;
    fj_wayland_get_global(client, object_id, &interface_id, &global);

    if (interface_id == FJ_WAYLAND_INTERFACE_MAX) {
        return;
    }

    if (fj_wayland_get_interface_type(interface_id) == FJ_WAYLAND_INTERFACE_DYNAMIC) {
        wayland_handle_dynamic_global(client, interface_id, global, false);
    }
}


static
struct wl_registry_listener const registry_listener = {
    .global = wayland_registry_add,
    .global_remove = wayland_registry_remove,
};


static
fj_err_t wayland_init_event_recording(struct fj_wayland_client * client)
{
    fj_vec_init(&client->recorded_events, sizeof(struct fj_wayland_event_wrapper));

    return fj_vec_allocate(&client->recorded_events);
}


static
void wayland_deinit_event_recording(struct fj_wayland_client * client)
{
    for (size_t i=0; i<client->recorded_events.length; i++) {
        struct fj_wayland_event_wrapper * wrapper = fj_vec_offset(&client->recorded_events, i);

        if (wrapper->event != 0) {
            FJ_FREE(&wrapper->event);
        }
    }

    fj_vec_deinit(&client->recorded_events);
}


static
fj_err_t wayland_record_events_pending(struct fj_wayland_client * client)
{
    if (wl_display_dispatch_pending(client->display) == -1) {
        return FJ_ERR_MESSAGE_READ_ERROR;
    }

    if (fj_wayland_record_failed(client)) {
        return FJ_ERR_OUT_OF_MEMORY;
    }

    return FJ_OK;
}


static
fj_err_t wayland_record_events_roundtrip(struct fj_wayland_client * client)
{
    if (wl_display_roundtrip(client->display) == -1) {
        return FJ_ERR_MESSAGE_READ_ERROR;
    }

    if (fj_wayland_record_failed(client)) {
        return FJ_ERR_OUT_OF_MEMORY;
    }

    return FJ_OK;
}


static
fj_bool32_t filter_all_events(
    struct fj_wayland_client * _client,
    void * _filter_callback_data,
    struct fj_wayland_event_wrapper const * _event_wrapper
)
{
    FJ_ARG_UNUSED(client)
    FJ_ARG_UNUSED(filter_callback_data)
    FJ_ARG_UNUSED(event_wrapper)

    return true;
}


static
fj_bool32_t filter_interface_init_events(
    struct fj_wayland_client * _client,
    void * _filter_callback_data,
    struct fj_wayland_event_wrapper const * event_wrapper
)
{
    FJ_ARG_UNUSED(client)
    FJ_ARG_UNUSED(filter_callback_data)

    return event_wrapper->event_group == FJ_WAYLAND_EVENT_GROUP_INTERFACE_INIT;
}


static
fj_err_t wayland_handle_event(
    struct fj_wayland_client * client,
    size_t index,
    struct fj_wayland_event_wrapper const * wrapper_ptr
)
{
    FJ_INIT_TRY

    struct fj_wayland_event_wrapper wrapper = *wrapper_ptr;

    FJ_TRY(fj_vec_remove(&client->recorded_events, index, 1)) {
        return FJ_RESULT;
    }

    FJ_TRY(wrapper.handle(client, &wrapper)) {
        if (wrapper.event != NULL) {
            FJ_FREE(&wrapper.event);
        }
        return FJ_RESULT;
    }

    if (wrapper.event != NULL) {
        FJ_FREE(&wrapper.event);
    }

    return FJ_OK;
}


fj_err_t fj_wayland_handle_events(
    struct fj_wayland_client * client,
    void * filter_callback_data,
    fj_wayland_event_filter_fn_t * event_filter
)
{
    FJ_INIT_TRY

    for (size_t i=0; i<client->recorded_events.length; ) {

        struct fj_wayland_event_wrapper const * wrapper;
        wrapper = fj_vec_offset(&client->recorded_events, i);

        if (event_filter(client, filter_callback_data, wrapper->event)) {

            FJ_TRY(wayland_handle_event(client, i, wrapper)) {
                return FJ_RESULT;
            }

        } else {
            i++;
        }
    }

    return FJ_OK;
}


fj_err_t fj_wayland_roundtrip(struct fj_wayland_client * client)
{
    return wayland_record_events_roundtrip(client);
}


fj_err_t fj_wayland_roundtrip_and_handle_events(
    struct fj_wayland_client * client,
    void * filter_callback_data,
    fj_wayland_event_filter_fn_t * event_filter
)
{
    FJ_INIT_TRY

    FJ_TRY(wayland_record_events_roundtrip(client)) {
        return FJ_RESULT;
    }

    FJ_TRY(fj_wayland_handle_events(client, filter_callback_data, event_filter)) {
        return FJ_RESULT;
    }

    return FJ_OK;
}


fj_err_t fj_wayland_bind_global(
    struct fj_wayland_client * client,
    fj_wayland_interface_id_t interface_id,
    struct fj_wayland_global const * global,
    void */*out*/ * object
)
{
    struct wl_interface const * interface = wayland_get_interface(interface_id);
    *object = wl_registry_bind(client->registry, global->id, interface, global->version);

    if (*object == NULL) {
        return FJ_ERR_INTERFACE_INIT_ERROR;
    }

    return FJ_OK;
}


static
fj_err_t wayland_handle_poll_event(
    void * client_,
    fj_unixpoller_fd_t _file_descriptor,
    fj_unixpoller_event_mask_t _event_mask
)
{
    FJ_ARG_FROM_OPAQUE(client, struct fj_wayland_client *)
    FJ_ARG_UNUSED(file_descriptor)
    FJ_ARG_UNUSED(event_mask)

    FJ_INIT_TRY

    if (wl_display_read_events(client->display) == -1) {
        return FJ_ERR_MESSAGE_READ_ERROR;
    }

    FJ_TRY(wayland_record_events_pending(client)) {
        return FJ_RESULT;
    }

    FJ_TRY(fj_wayland_handle_events(client, NULL, filter_all_events)) {
        return FJ_RESULT;
    }

    return FJ_OK;
}


static
fj_err_t wayland_wait_display_writable(struct fj_wayland_client * client)
{
    struct pollfd p = {
        .fd = wl_display_get_fd(client->display),
        .revents = POLLOUT,
    };

    if (poll(&p, 1, -1) == -1) {
        return FJ_ERR_IO_ERROR;
    }

    return FJ_OK;
}


static
fj_err_t wayland_display_flush(struct fj_wayland_client * client)
{
    FJ_INIT_TRY

    while (wl_display_flush(client->display) != 0) {
        if (errno != EAGAIN) {
            return FJ_ERR_IO_ERROR;
        }

        FJ_TRY(wayland_wait_display_writable(client)) {
            return FJ_RESULT;
        }
	}

    return FJ_OK;
}


static
fj_err_t wayland_prepare_poll(struct fj_wayland_client * client)
{
    FJ_INIT_TRY

    while (wl_display_prepare_read(client->display) != 0) {
        FJ_TRY(wayland_record_events_pending(client)) {
            return FJ_RESULT;
        }

        FJ_TRY(fj_wayland_handle_events(client, NULL, filter_all_events)) {
            return FJ_RESULT;
        }
    }

	FJ_TRY(wayland_display_flush(client)) {
        wl_display_cancel_read(client->display);
        return FJ_RESULT;
    }

    return FJ_OK;
}


fj_err_t fj_wayland_record_event(
    struct fj_wayland_client * client,
    struct fj_wayland_event_wrapper const * wrapper
)
{
    FJ_INIT_TRY

    if (fj_wayland_record_failed(client)) {
        return FJ_ERR_OUT_OF_MEMORY;
    }

    struct fj_wayland_event_wrapper new_wrapper = *wrapper;

    if (wrapper->event_size != 0) {
        FJ_TRY(fj_alloc_copied(
            (void **) &new_wrapper.event,
            wrapper->event,
            wrapper->event_size
        )) {
            return FJ_RESULT;
        }
    }

    FJ_TRY(fj_vec_push(&client->recorded_events, &new_wrapper)) {
        if (wrapper->event != NULL) {
            FJ_FREE(&new_wrapper.event);
        }
    }

    return FJ_OK;
}


void fj_wayland_record_fail(struct fj_wayland_client * client)
{
    wayland_deinit_event_recording(client);
}


fj_bool32_t fj_wayland_record_failed(struct fj_wayland_client * client)
{
    return !fj_vec_has_allocated(&client->recorded_events);
}


static
fj_err_t wayland_connect(struct fj_wayland_client * client)
{
    FJ_INIT_TRY

    client->display = wl_display_connect(NULL);

    if (client->display == NULL) {
        return FJ_ERR_SHELL_CONNECTION_ERROR;
    }

    FJ_TRY(fj_unixpoller_add(
        &client->unixpoller,
        wl_display_get_fd(client->display),
        POLLIN,
        wayland_handle_poll_event
    )) {
        wl_display_disconnect(client->display);
        return FJ_RESULT;
    }

    return FJ_OK;
}


static
fj_err_t wayland_init_registry(struct fj_wayland_client * client)
{
    FJ_INIT_TRY

    client->registry = wl_display_get_registry(client->display);

    if (client->registry == NULL) {
        return FJ_ERR_REQUEST_FAILED;
    }

    wl_registry_add_listener(client->registry, &registry_listener, client);

    FJ_TRY(fj_wayland_roundtrip_and_handle_events(client, NULL, filter_all_events)) {
        return FJ_RESULT;
    }

    return FJ_OK;
}


static
void wayland_init_global_lists(struct fj_wayland_client * client)
{
    for (uint32_t i=0; i<FJ_ARRAY_LEN(client->interfaces); i++) {
        if (fj_wayland_get_interface_type(i) == FJ_WAYLAND_INTERFACE_DYNAMIC) {
            fj_vec_init(&client->interfaces[i].global_list, sizeof(struct fj_wayland_global));
        }
    }
}


static
void wayland_deinit_global_lists(struct fj_wayland_client * client)
{
    for (uint32_t i=0; i<FJ_ARRAY_LEN(client->interfaces); i++) {
        if (fj_wayland_get_interface_type(i) == FJ_WAYLAND_INTERFACE_DYNAMIC) {
            fj_vec_deinit(&client->interfaces[i].global_list);
        }
    }
}


static
fj_err_t wayland_init(struct fj_wayland_client * client)
{
    FJ_INIT_TRY

    FJ_TRY(wayland_connect(client)) {
        return FJ_RESULT;
    }

    wayland_init_global_lists(client);

    FJ_TRY(wayland_init_event_recording(client)) {
        wl_display_disconnect(client->display);
        return FJ_RESULT;
    }

    FJ_TRY(wayland_init_registry(client)) {
        wl_display_disconnect(client->display);
        wayland_deinit_event_recording(client);
        return FJ_RESULT;
    }

    return FJ_OK;
}


static
void wayland_deinit(struct fj_wayland_client * client)
{
    wayland_deinit_event_recording(client);

    wayland_deinit_global_lists(client);

    wl_display_disconnect(client->display);
}


static
fj_err_t client_init_run(struct fj_wayland_client * client)
{
    return client->callbacks->idle(client->data);
}


static
fj_err_t client_prepare_poll(struct fj_wayland_client * client)
{
    return wayland_prepare_poll(client);
}


static
fj_err_t client_end_poll(struct fj_wayland_client * client)
{
    return client->callbacks->idle(client->data);
}


static
void client_cancel_poll(struct fj_wayland_client * client)
{
    wl_display_cancel_read(client->display);
}


static
fj_err_t client_cleanup_interfaces(struct fj_wayland_client * client)
{
    FJ_INIT_TRY

    fj_err_t first_error = FJ_OK;

    for (uint32_t i=0; i<FJ_ARRAY_LEN(cleanup_functions); i++) {
        FJ_TRY(cleanup_functions[i](client)) {
            if (first_error == FJ_OK) {
                first_error = FJ_RESULT;
            }
        }
    }

    return first_error;
}


static
fj_err_t client_init(struct fj_wayland_client * client, struct fj_client_info const * info)
{
    FJ_INIT_TRY

    FJ_TRY(fj_strdup(info->name, &client->name)) {
        return FJ_RESULT;
    }

    FJ_TRY(fj_unixpoller_init(&client->unixpoller, (void *) client)) {
        FJ_FREE(&client->name);
        return FJ_RESULT;
    }

    FJ_TRY(wayland_init(client)) {
        FJ_FREE(&client->name);
        fj_unixpoller_deinit(&client->unixpoller);
        return FJ_RESULT;
    }

    return FJ_OK;
}


static
fj_err_t client_deinit(struct fj_wayland_client * client)
{
    fj_err_t result = client_cleanup_interfaces(client);

    wayland_deinit(client);
    fj_unixpoller_deinit(&client->unixpoller);
    FJ_FREE(&client->name);

    return result;
}


static
fj_err_t client_create(
    fj_client_t */*? out*/ * client_,
    struct fj_client_callbacks const * callbacks,
    void * callback_data,
    struct fj_client_info const * info
)
{
    FJ_ARG_FROM_OPAQUE(client, struct fj_wayland_client */*? out*/ *)

    FJ_INIT_TRY

    FJ_TRY(FJ_ALLOC_ZEROED(client)) {
        return FJ_RESULT;
    }

    (*client)->callbacks = callbacks;
    (*client)->data = callback_data;

    FJ_TRY(client_init(*client, info)) {
        FJ_FREE(&client);
        return FJ_RESULT;
    }

    return FJ_OK;
}


static
fj_err_t client_destroy(fj_client_t * client_)
{
    FJ_ARG_FROM_OPAQUE(client, struct fj_wayland_client *)

    fj_err_t result = client_deinit(client);

    FJ_FREE(&client);

    return result;
}


static
fj_err_t client_run(fj_client_t * client_, fj_client_run_type_t run_type, void * _serve_data)
{
    FJ_ARG_FROM_OPAQUE(client, struct fj_wayland_client *)
    FJ_ARG_UNUSED(serve_data)

    FJ_INIT_TRY

    if (run_type != FJ_CLIENT_RUN_MAIN) {
        return FJ_OK;
    }

    FJ_TRY(client_init_run(client)) {
        return FJ_RESULT;
    }

    while (!fj_unixpoller_should_finish(&client->unixpoller)) {
        FJ_TRY(client_prepare_poll(client)) {
            return FJ_RESULT;
        }

        FJ_TRY(fj_unixpoller_poll(&client->unixpoller)) {
            client_cancel_poll(client);
            return FJ_RESULT;
        }

        FJ_TRY(client_end_poll(client)) {
            return FJ_RESULT;
        }
    }

    return FJ_OK;
}


static
fj_err_t client_commit(fj_client_t * client_)
{
    FJ_ARG_FROM_OPAQUE(client, struct fj_wayland_client *)

    return fj_wayland_roundtrip_and_handle_events(client, NULL, filter_interface_init_events);
}



static
void client_set_sleep_timeout(fj_client_t * client_, fj_seconds_t timeout)
{
    FJ_ARG_FROM_OPAQUE(client, struct fj_wayland_client *)

    fj_unixpoller_set_timeout(&client->unixpoller, timeout);
}


static
fj_err_t client_wakeup(fj_client_t * client_)
{
    FJ_ARG_FROM_OPAQUE(client, struct fj_wayland_client *)

    return fj_unixpoller_wakeup(&client->unixpoller);
}


struct fj_client_iface const fj_wayland_client_iface = {
    .create = client_create,
    .destroy = client_destroy,
    .run = client_run,
    .commit = client_commit,
    .set_sleep_timeout = client_set_sleep_timeout,
    .wakeup = client_wakeup,
};
