#include <fejix/core/sys.h>
#include <fejix/core/list.h>
#include <fejix/core/map.h>
#include <fejix/core/utils.h>

#include <malloc.h>


struct fj_sys {
    /// interface_id -> *interface
    struct fj_map * interfaces;

    /// entity_id -> interface_id -> *resource
    struct fj_map * resources;

    /// entity_id -> event_id -> modules_that_handle_the_event[]
    struct fj_map * event_bindings;
};


struct find_interface_data {
    fj_ptr_t interface;
    fj_id_t interface_id;
};


static fj_err_t sys_init(struct fj_sys * sys)
{
    sys->interfaces = fj_map_new();
    sys->resources = fj_map_new();
    sys->event_bindings = fj_map_new();

    if (sys->interfaces == NULL
        || sys->resources == NULL
        || sys->event_bindings == NULL
    ) {
        return FJ_ERR(FJ_MALLOC_FAILED);
    }

    return FJ_OK;
}


static fj_map_foreach_result_t destroy_map(
    struct fj_map_element * element,
    fj_ptr_t data
)
{
    (void) data;

    fj_map_del(element->value);
    return FJ_MAP_FOREACH_CONTINUE;
}


static fj_map_foreach_result_t destroy_list(
    struct fj_map_element * element,
    fj_ptr_t data
)
{
    (void) data;

    fj_list_del(element->value);
    return FJ_MAP_FOREACH_CONTINUE;
}


static void sys_destroy(struct fj_sys * sys)
{
    if (sys->interfaces != NULL) {
        fj_map_del(sys->interfaces);
    }

    if (sys->resources != NULL) {
        fj_map_foreach(sys->resources, destroy_map, NULL);
        fj_map_del(sys->resources);
    }

    if (sys->event_bindings != NULL) {
        fj_map_foreach(sys->event_bindings, destroy_list, NULL);
        fj_map_del(sys->event_bindings);
    }
}


static struct fj_map * insert_new_map(struct fj_map * map, fj_id_t key)
{
    struct fj_map * new_map = fj_map_new();

    if (new_map == NULL) {
        return NULL;
    }

    if (fj_map_set(map, key, new_map) != FJ_OK) {
        fj_map_del(new_map);
        return NULL;
    }

    return new_map;
}


static struct fj_map * get_or_insert_new_map(struct fj_map * map, fj_id_t key)
{
    struct fj_map * submap = fj_map_get(map, key);

    if (submap == NULL) {
        submap = insert_new_map(map, key);
    }

    return submap;
}


static struct fj_list * insert_new_list(struct fj_map * map, fj_id_t key)
{
    struct fj_list * new_list = fj_list_new();

    if (new_list == NULL) {
        return NULL;
    }

    if (fj_map_set(map, key, new_list) != FJ_OK) {
        fj_list_del(new_list);
        return NULL;
    }

    return new_list;
}


static struct fj_list * get_or_insert_new_list(struct fj_map * map, fj_id_t key)
{
    struct fj_list * list = fj_map_get(map, key);

    if (list == NULL) {
        list = insert_new_list(map, key);
    }

    return list;
}


static struct fj_list * get_or_create_handlers(
    struct fj_sys * sys,
    fj_id_t entity_id,
    fj_id_t event_id
)
{
    struct fj_map * bindings = get_or_insert_new_map(
        sys->event_bindings, entity_id
    );

    if (bindings == NULL) {
        return NULL;
    }

    struct fj_list * handlers = get_or_insert_new_list(bindings, event_id);

    return handlers;
}


static struct fj_list * get_handlers(
    struct fj_sys * sys,
    fj_id_t entity_id,
    fj_id_t event_id
)
{
    struct fj_map * bindings = fj_map_get(sys->event_bindings, entity_id);

    if (bindings == NULL) {
        return NULL;
    }

    return fj_map_get(bindings, event_id);
}


struct fj_sys * fj_sys_new(void)
{
    struct fj_sys * sys = calloc(1, sizeof(struct fj_sys));

    if (sys == NULL) {
        return NULL;
    }

    if (sys_init(sys) != FJ_OK) {
        fj_sys_del(sys);
        return NULL;
    }

    return sys;
}


void fj_sys_del(struct fj_sys * sys)
{
    sys_destroy(sys);
    free(sys);
}


fj_err_t fj_sys_set_interface(
    struct fj_sys * sys,
    fj_id_t interface_id,
    fj_ptr_t interface
)
{
    return fj_map_set(sys->interfaces, interface_id, interface);
}


fj_ptr_t fj_sys_get_interface(
    struct fj_sys * sys,
    fj_id_t interface_id
)
{
    return fj_map_get(sys->interfaces, interface_id);
}


// TODO If all resources of an entity get deleted, the map for storing those
// resources is not freed. Do we need to fix this?
fj_err_t fj_sys_set_resource(
    struct fj_sys * sys,
    fj_id_t entity_id,
    fj_id_t interface_id,
    fj_ptr_t resource
)
{
    struct fj_map * resources = get_or_insert_new_map(
        sys->resources, entity_id
    );

    if (resources == NULL) {
        return FJ_ERR(FJ_MALLOC_FAILED);
    }

    return fj_map_set(resources, interface_id, resource);
}


fj_ptr_t fj_sys_get_resource(
    struct fj_sys * sys,
    fj_id_t entity_id,
    fj_id_t interface_id
)
{
    struct fj_map * resources = fj_map_get(sys->resources, entity_id);

    if (resources == NULL) {
        return NULL;
    }

    return fj_map_get(resources, interface_id);
}


fj_err_t fj_sys_bind_event(
    struct fj_sys * sys,
    fj_id_t entity_id,
    fj_id_t event_id,
    fj_id_t handler_interface_id
)
{
    struct fj_list * handlers = get_or_create_handlers(
        sys, entity_id, event_id
    );

    if (handlers == NULL) {
        return FJ_ERR(FJ_MALLOC_FAILED);
    }

    return fj_list_include(handlers, handler_interface_id);
}


// TODO If the last handler gets unbound from an entity, the list for storing
// its bindings is not freed. Do we need to fix this?
fj_err_t fj_sys_unbind_event(
    struct fj_sys * sys,
    fj_id_t entity_id,
    fj_id_t event_id,
    fj_id_t handler_interface_id
)
{
    struct fj_list * handlers = get_handlers(sys, entity_id, event_id);

    if (handlers == NULL) {
        return FJ_OK;
    }

    return fj_list_exclude(handlers, handler_interface_id);
}


static fj_err_t invoke_handler(
    struct fj_sys * sys,
    fj_id_t handler,
    struct fj_event_data * event_data
)
{
    struct fj_event_handler * interface = fj_sys_get_interface(sys, handler);

    if (interface == NULL) {
        return FJ_ERR("cannot call event handler (interface not found)");
    }

    return interface->handle_event(sys, event_data);
}


static fj_err_t handle_event(
    struct fj_sys * sys,
    struct fj_list * handlers,
    struct fj_event_data * event_data
)
{
    fj_err_t e = FJ_OK;

    for (uint32_t i = 0; i < handlers->length; i++) {
        e = invoke_handler(sys, handlers->elements[i], event_data);

        if (e != FJ_OK) {
            break;
        }
    }

    return e;
}


fj_err_t fj_sys_emit_event(
    struct fj_sys * sys,
    fj_id_t entity_id,
    fj_id_t event_id,
    fj_ptr_t event
)
{
    struct fj_event_data event_data = { 0 };
    event_data.entity_id = entity_id;
    event_data.event_id = event_id;
    event_data.event = event;

    struct fj_list * handlers = get_handlers(sys, entity_id, event_id);

    if (handlers == NULL) {
        return FJ_OK;
    }

    struct fj_list * handlers_clone = fj_list_clone(handlers);

    if (handlers_clone == NULL) {
        return FJ_ERR(FJ_MALLOC_FAILED);
    }

    fj_err_t e = handle_event(sys, handlers_clone, &event_data);

    fj_list_del(handlers_clone);

    return e;
}
