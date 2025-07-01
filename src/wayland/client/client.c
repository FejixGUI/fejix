#include <src/wayland/client/client.h>

#include <fejix/core/alloc.h>
#include <fejix/core/utils.h>

#include <errno.h>
#include <string.h>

#include <wayland-client-core.h>
#include <wayland-client-protocol.h>


struct wayland_dynamic_global_event {
    struct fj_wayland_global global;
    fj_wayland_interface_id interface_id;
    fj_bool8 added;
};


/** Contains interface strings or NULL (if unimplemented). */
static char const *const wayland_interface_names[] = {
    [FJ_WAYLAND_INTERFACE_MAX] = "...static global max ID...",

    [FJ_WAYLAND_INTERFACE_COMPOSITOR] = "wl_compositor",
    [FJ_WAYLAND_INTERFACE_SHM] = "wl_shm",
    [FJ_WAYLAND_INTERFACE_SEAT] = "wl_seat",
    [FJ_WAYLAND_INTERFACE_OUTPUT] = "wl_output",
};

static fj_wayland_interface_type const wayland_interface_types[] = {
    [FJ_WAYLAND_INTERFACE_MAX] = FJ_WAYLAND_INTERFACE_STATIC,

    [FJ_WAYLAND_INTERFACE_COMPOSITOR] = FJ_WAYLAND_INTERFACE_STATIC,
    [FJ_WAYLAND_INTERFACE_SHM] = FJ_WAYLAND_INTERFACE_STATIC,
    [FJ_WAYLAND_INTERFACE_SEAT] = FJ_WAYLAND_INTERFACE_DYNAMIC,
    [FJ_WAYLAND_INTERFACE_OUTPUT] = FJ_WAYLAND_INTERFACE_DYNAMIC,
};


static struct wl_interface const *const wayland_interfaces[] = {
    [FJ_WAYLAND_INTERFACE_MAX] = NULL,

    [FJ_WAYLAND_INTERFACE_COMPOSITOR] = &wl_compositor_interface,
    [FJ_WAYLAND_INTERFACE_SHM] = &wl_shm_interface,
    [FJ_WAYLAND_INTERFACE_SEAT] = &wl_seat_interface,
    [FJ_WAYLAND_INTERFACE_OUTPUT] = &wl_output_interface,
};


/** Returns FJ_WAYLAND_INTERFACE_MAX if the global is not found in the list. */
static uint32_t wayland_get_interface_id(char const * /*[]*/ interface_name)
{
    for (uint32_t i = 0; i < FJ_LEN(wayland_interface_names); i++) {
        if (wayland_interface_names[i] == NULL) {
            continue;
        }

        if (fj_str_eq(wayland_interface_names[i], interface_name)) {
            return i;
        }
    }

    return FJ_WAYLAND_INTERFACE_MAX;
}


fj_wayland_interface_type fj_wayland_get_interface_type(fj_wayland_interface_id interface_id)
{
    return wayland_interface_types[interface_id];
}


struct wl_interface const *wayland_get_interface(fj_wayland_interface_id interface_id)
{
    return wayland_interfaces[interface_id];
}


void fj_wayland_get_global_by_id(
    struct fj_client *client,
    uint32_t object_id,
    fj_wayland_interface_id /*out*/ *out_interface_id,
    struct fj_wayland_global const * /*? out*/ *out_global)
{
    *out_interface_id = FJ_WAYLAND_INTERFACE_MAX;
    *out_global = NULL;

    for (uint32_t i = 0; i < FJ_LEN(client->interfaces); i++) {
        switch (fj_wayland_get_interface_type(i)) {
            case FJ_WAYLAND_INTERFACE_STATIC:
                if (client->interfaces[i].global.id == object_id) {
                    *out_interface_id = i;
                    *out_global = &client->interfaces[i].global;
                }
                break;

            case FJ_WAYLAND_INTERFACE_DYNAMIC:
                for (uint32_t j = 0; j < client->interfaces[i].global_list.length; j++) {
                    struct fj_wayland_global const *global
                        = fj_vec_offset(&client->interfaces[i].global_list, j);

                    if (global->id == object_id) {
                        *out_interface_id = i;
                        *out_global = global;
                    }
                }
                break;
        }
    }
}


struct fj_wayland_global const * /*?*/ fj_wayland_get_static_global(
    struct fj_client *client, fj_wayland_interface_id interface_id)
{
    return &client->interfaces[interface_id].global;
}


static enum fj_status wayland_handle_dynamic_global_event(
    struct fj_client *client, struct fj_wayland_event_wrapper const *event_wrapper)
{
    struct wayland_dynamic_global_event const *event = event_wrapper->event;

    struct fj_vec *global_list = &client->interfaces[event->interface_id].global_list;

    if (event->added) {
        FJ_TRY (fj_vec_push(global_list, &event->global)) {
            return fj_result;
        }
    }

    // TODO: Delegate Wayland dynamic global events to specialised interfaces (if initialised).

    if (!event->added) {
        for (uint32_t i = 0; i < global_list->length; i++) {
            struct fj_wayland_global const *global = fj_vec_offset(global_list, i);
            if (global->id == event->global.id) {
                FJ_TRY (fj_vec_remove(global_list, i, 1)) {
                    return fj_result;
                }
            }
        }
    }

    return FJ_OK;
}


static void wayland_handle_static_global(
    struct fj_client *client,
    fj_wayland_interface_id interface_id,
    struct fj_wayland_global const *global)
{
    client->interfaces[interface_id].global = *global;
}


static void wayland_handle_dynamic_global(
    struct fj_client *client,
    fj_wayland_interface_id interface_id,
    struct fj_wayland_global const *global,
    fj_bool8 added)
{
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

    FJ_TRY (fj_wayland_record_event(client, &wrapper)) {
        fj_wayland_record_fail(client);
        return;
    }
}


static void wayland_registry_add(
    void *_client,
    struct wl_registry *registry,
    uint32_t object_id,
    char const *interface_name,
    uint32_t interface_version)
{
    struct fj_client *client = (void *) _client;
    (void) registry;

    struct fj_wayland_global const global = {
        .id = object_id,
        .version = interface_version,
    };

    fj_wayland_interface_id interface_id = wayland_get_interface_id(interface_name);

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


static void wayland_registry_remove(void *_client, struct wl_registry *registry, uint32_t object_id)
{
    struct fj_client *client = (void *) _client;
    (void) registry;

    uint32_t interface_id;
    struct fj_wayland_global const *global;
    fj_wayland_get_global_by_id(client, object_id, &interface_id, &global);

    if (global == NULL) {
        return;
    }

    if (fj_wayland_get_interface_type(interface_id) == FJ_WAYLAND_INTERFACE_DYNAMIC) {
        wayland_handle_dynamic_global(client, interface_id, global, false);
    }
}


static struct wl_registry_listener const registry_listener = {
    .global = wayland_registry_add,
    .global_remove = wayland_registry_remove,
};


static enum fj_status wayland_init_event_recording(struct fj_client *client)
{
    fj_vec_init(&client->recorded_events, sizeof(struct fj_wayland_event_wrapper));

    return fj_vec_allocate(&client->recorded_events);
}


static void wayland_deinit_event_recording(struct fj_client *client)
{
    for (size_t i = 0; i < client->recorded_events.length; i++) {
        struct fj_wayland_event_wrapper *wrapper = fj_vec_offset(&client->recorded_events, i);

        if (wrapper->event != 0) {
            FJ_FREE(&wrapper->event);
        }
    }

    fj_vec_deinit(&client->recorded_events);
}


static enum fj_status wayland_record_events_waited(struct fj_client *client)
{
    if (wl_display_dispatch(client->display) == -1) {
        return FJ_ERROR_EVENT_READING_FAILED;
    }

    if (fj_wayland_record_failed(client)) {
        return FJ_ERROR_ALLOCATION_FAILED;
    }

    return FJ_OK;
}


static enum fj_status wayland_record_events_pending(struct fj_client *client)
{
    if (wl_display_dispatch_pending(client->display) == -1) {
        return FJ_ERROR_EVENT_READING_FAILED;
    }

    if (fj_wayland_record_failed(client)) {
        return FJ_ERROR_ALLOCATION_FAILED;
    }

    return FJ_OK;
}


static enum fj_status wayland_record_events_roundtrip(struct fj_client *client)
{
    if (wl_display_roundtrip(client->display) == -1) {
        return FJ_ERROR_EVENT_READING_FAILED;
    }

    if (fj_wayland_record_failed(client)) {
        return FJ_ERROR_ALLOCATION_FAILED;
    }

    return FJ_OK;
}


static fj_bool8 filter_all_events(
    struct fj_client *client,
    void *filter_callback_data,
    struct fj_wayland_event_wrapper const *event_wrapper)
{
    (void) client;
    (void) filter_callback_data;
    (void) event_wrapper;

    return true;
}


static fj_bool8 filter_interface_init_events(
    struct fj_client *client,
    void *filter_callback_data,
    struct fj_wayland_event_wrapper const *event_wrapper)
{
    (void) client;
    (void) filter_callback_data;

    return event_wrapper->event_group == FJ_WAYLAND_EVENT_GROUP_INTERFACE_INIT;
}


static enum fj_status wayland_handle_event(
    struct fj_client *client, size_t index, struct fj_wayland_event_wrapper const *wrapper_ptr)
{
    struct fj_wayland_event_wrapper wrapper = *wrapper_ptr;

    FJ_TRY (fj_vec_remove(&client->recorded_events, index, 1)) {
        return fj_result;
    }

    FJ_TRY (wrapper.handle(client, &wrapper)) {
        if (wrapper.event != NULL) {
            FJ_FREE(&wrapper.event);
        }
        return fj_result;
    }

    if (wrapper.event != NULL) {
        FJ_FREE(&wrapper.event);
    }

    return FJ_OK;
}


enum fj_status fj_wayland_handle_events(
    struct fj_client *client, void *filter_callback_data, fj_wayland_event_filter_fn *event_filter)
{
    for (size_t i = 0; i < client->recorded_events.length;) {
        struct fj_wayland_event_wrapper const *wrapper = fj_vec_offset(&client->recorded_events, i);

        if (event_filter(client, filter_callback_data, wrapper)) {
            FJ_TRY (wayland_handle_event(client, i, wrapper)) {
                return fj_result;
            }

        } else {
            i++;
        }
    }

    return FJ_OK;
}


enum fj_status fj_wayland_roundtrip(struct fj_client *client)
{
    return wayland_record_events_roundtrip(client);
}


enum fj_status fj_wayland_wait_for_events(struct fj_client *client)
{
    return wayland_record_events_waited(client);
}


enum fj_status fj_wayland_bind_global(
    struct fj_client *client,
    fj_wayland_interface_id interface_id,
    struct fj_wayland_global const *global,
    void * /*? out*/ *object)
{
    struct wl_interface const *interface = wayland_get_interface(interface_id);
    *object = wl_registry_bind(client->registry, global->id, interface, global->version);

    if (*object == NULL) {
        return FJ_ERROR_REQUEST_FAILED;
    }

    return FJ_OK;
}


static enum fj_status wayland_handle_poll_event(
    void *_client, fj_unixpoller_fd file_descriptor, fj_unixpoller_event_mask event_mask)
{
    struct fj_client *client = (void *) _client;
    (void) file_descriptor;
    (void) event_mask;

    if (wl_display_read_events(client->display) == -1) {
        return FJ_ERROR_EVENT_READING_FAILED;
    }

    FJ_TRY (wayland_record_events_pending(client)) {
        return fj_result;
    }

    FJ_TRY (fj_wayland_handle_events(client, NULL, filter_all_events)) {
        return fj_result;
    }

    return FJ_OK;
}


static enum fj_status wayland_wait_display_writable(struct fj_client *client)
{
    struct pollfd p = {
        .fd = wl_display_get_fd(client->display),
        .revents = POLLOUT,
    };

    if (poll(&p, 1, -1) == -1) {
        return FJ_ERROR_EVENT_WAITING_FAILED;
    }

    return FJ_OK;
}


static enum fj_status wayland_display_flush(struct fj_client *client)
{
    while (wl_display_flush(client->display) != 0) {
        if (errno != EAGAIN) {
            return FJ_ERROR_EVENT_WAITING_FAILED;
        }

        FJ_TRY (wayland_wait_display_writable(client)) {
            return fj_result;
        }
    }

    return FJ_OK;
}


static enum fj_status wayland_prepare_poll(struct fj_client *client)
{
    while (wl_display_prepare_read(client->display) != 0) {
        FJ_TRY (wayland_record_events_pending(client)) {
            return fj_result;
        }

        FJ_TRY (fj_wayland_handle_events(client, NULL, filter_all_events)) {
            return fj_result;
        }
    }

    FJ_TRY (wayland_display_flush(client)) {
        wl_display_cancel_read(client->display);
        return fj_result;
    }

    return FJ_OK;
}


enum fj_status fj_wayland_record_event(
    struct fj_client *client, struct fj_wayland_event_wrapper const *wrapper)
{
    if (fj_wayland_record_failed(client)) {
        return FJ_ERROR_ALLOCATION_FAILED;
    }

    struct fj_wayland_event_wrapper new_wrapper = *wrapper;

    if (wrapper->event_size != 0) {
        FJ_TRY (fj_alloc_copied((void **) &new_wrapper.event, wrapper->event, wrapper->event_size))
        {
            return fj_result;
        }
    }

    FJ_TRY (fj_vec_push(&client->recorded_events, &new_wrapper)) {
        if (wrapper->event != NULL) {
            FJ_FREE(&new_wrapper.event);
        }
    }

    return FJ_OK;
}


void fj_wayland_record_fail(struct fj_client *client)
{
    wayland_deinit_event_recording(client);
}


fj_bool8 fj_wayland_record_failed(struct fj_client *client)
{
    return !fj_vec_has_allocated(&client->recorded_events);
}


static enum fj_status wayland_connect(struct fj_client *client)
{
    client->display = wl_display_connect(NULL);

    if (client->display == NULL) {
        return FJ_ERROR_IO_THREAD_FAILED;
    }

    FJ_TRY (fj_unixpoller_add(
                &client->unixpoller,
                wl_display_get_fd(client->display),
                POLLIN,
                wayland_handle_poll_event))
    {
        wl_display_disconnect(client->display);
        return fj_result;
    }

    return FJ_OK;
}


static enum fj_status wayland_init_registry(struct fj_client *client)
{
    client->registry = wl_display_get_registry(client->display);

    if (client->registry == NULL) {
        return FJ_ERROR_REQUEST_FAILED;
    }

    wl_registry_add_listener(client->registry, &registry_listener, client);

    FJ_TRY (fj_wayland_roundtrip(client)) {
        return fj_result;
    }

    return fj_wayland_handle_events(client, NULL, filter_all_events);
}


static void wayland_init_global_lists(struct fj_client *client)
{
    for (uint32_t i = 0; i < FJ_LEN(client->interfaces); i++) {
        if (fj_wayland_get_interface_type(i) == FJ_WAYLAND_INTERFACE_DYNAMIC) {
            fj_vec_init(&client->interfaces[i].global_list, sizeof(struct fj_wayland_global));
        }
    }
}


static void wayland_deinit_global_lists(struct fj_client *client)
{
    for (uint32_t i = 0; i < FJ_LEN(client->interfaces); i++) {
        if (fj_wayland_get_interface_type(i) == FJ_WAYLAND_INTERFACE_DYNAMIC) {
            fj_vec_deinit(&client->interfaces[i].global_list);
        }
    }
}


static enum fj_status wayland_init(struct fj_client *client)
{
    FJ_TRY (wayland_connect(client)) {
        return fj_result;
    }

    wayland_init_global_lists(client);

    FJ_TRY (wayland_init_event_recording(client)) {
        wl_display_disconnect(client->display);
        return fj_result;
    }

    FJ_TRY (wayland_init_registry(client)) {
        wl_display_disconnect(client->display);
        wayland_deinit_event_recording(client);
        return fj_result;
    }

    return FJ_OK;
}


static void wayland_deinit(struct fj_client *client)
{
    wayland_deinit_event_recording(client);

    wayland_deinit_global_lists(client);

    wl_display_disconnect(client->display);
}


static enum fj_status client_init_run(struct fj_client *client)
{
    return client->callbacks.idle(client->data);
}


static enum fj_status client_prepare_poll(struct fj_client *client)
{
    return wayland_prepare_poll(client);
}


static enum fj_status client_end_poll(struct fj_client *client)
{
    return client->callbacks.idle(client->data);
}


static void client_cancel_poll(struct fj_client *client)
{
    wl_display_cancel_read(client->display);
}


static enum fj_status client_init(struct fj_client *client, struct fj_client_info const *info)
{
    FJ_TRY (fj_str_clone(info->name, &client->name)) {
        return fj_result;
    }

    FJ_TRY (fj_unixpoller_init(&client->unixpoller, (void *) client)) {
        FJ_FREE(&client->name);
        return fj_result;
    }

    FJ_TRY (wayland_init(client)) {
        FJ_FREE(&client->name);
        fj_unixpoller_deinit(&client->unixpoller);
        return fj_result;
    }

    return FJ_OK;
}


static enum fj_status client_deinit(struct fj_client *client)
{
    wayland_deinit(client);
    fj_unixpoller_deinit(&client->unixpoller);
    FJ_FREE(&client->name);
    return FJ_OK;
}


static enum fj_status client_create(
    struct fj_client * /*? out*/ *_client,
    struct fj_client_callbacks const *callbacks,
    void *callback_data,
    struct fj_client_info const *info)
{
    struct fj_client **client = (void *) _client;

    FJ_TRY (FJ_ALLOC(client)) {
        return fj_result;
    }

    (*client)->callbacks = *callbacks;
    (*client)->data = callback_data;

    FJ_TRY (client_init(*client, info)) {
        FJ_FREE(&client);
        return fj_result;
    }

    return FJ_OK;
}


static enum fj_status client_destroy(struct fj_client *_client)
{
    struct fj_client *client = (void *) _client;

    enum fj_status result = client_deinit(client);

    FJ_FREE(&client);

    return result;
}


static enum fj_status client_run(
    struct fj_client *_client, fj_client_run_type run_type, void *serve_data)
{
    struct fj_client *client = (void *) _client;
    (void) serve_data;

    if (run_type != FJ_CLIENT_RUN_MAIN) {
        return FJ_OK;
    }

    FJ_TRY (client_init_run(client)) {
        return fj_result;
    }

    while (!fj_unixpoller_should_finish(&client->unixpoller)) {
        FJ_TRY (client_prepare_poll(client)) {
            return fj_result;
        }

        FJ_TRY (fj_unixpoller_poll(&client->unixpoller)) {
            client_cancel_poll(client);
            return fj_result;
        }

        FJ_TRY (client_end_poll(client)) {
            return fj_result;
        }
    }

    return FJ_OK;
}


static enum fj_status client_commit(struct fj_client *_client)
{
    struct fj_client *client = (void *) _client;

    FJ_TRY (fj_wayland_roundtrip(client)) {
        return fj_result;
    }

    return fj_wayland_handle_events(client, NULL, filter_interface_init_events);
}


static void client_set_sleep_timeout(struct fj_client *_client, fj_seconds timeout)
{
    struct fj_client *client = (void *) _client;

    fj_unixpoller_set_timeout(&client->unixpoller, timeout);
}


static enum fj_status client_wakeup(struct fj_client *_client)
{
    struct fj_client *client = (void *) _client;

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
