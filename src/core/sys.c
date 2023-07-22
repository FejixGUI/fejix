#include <fejix/core/sys.h>

#include <malloc.h>


static fj_result_t sys_init(fj_sys_t * sys)
{
    sys->service_interfaces = fj_map_new();
    sys->entity_data = fj_map_new();
    sys->event_bindings = fj_map_new();

    if (
        sys->service_interfaces == NULL
        || sys->entity_data == NULL
        || sys->event_bindings == NULL
    )
    {
        return FJ_ALLOCATION_FAILED;
    }

    return FJ_OK;
}


static void sys_destroy_service_interfaces(fj_sys_t * sys)
{
    fj_map_iter_t iter;
    fj_map_iter_init(sys->service_interfaces, &iter);
    while (1) {
        fj_map_element_t * elem = fj_map_iter_next(&iter);

        if (elem == NULL) {
            break;
        }

        fj_map_t * interfaces = elem->value;
        fj_map_del(interfaces);
    }

    fj_map_del(sys->service_interfaces);
    sys->service_interfaces = NULL;
}


static void sys_destroy_entity_data(fj_sys_t * sys)
{
    fj_map_iter_t iter;
    fj_map_iter_init(sys->entity_data, &iter);
    while (1) {
        fj_map_element_t * elem = fj_map_iter_next(&iter);

        if (elem == NULL) {
            break;
        }

        fj_map_t * data = elem->value;
        fj_map_del(data);
    }

    fj_map_del(sys->service_interfaces);
    sys->service_interfaces = NULL;
}


static void sys_destroy_event_bindings(fj_sys_t * sys)
{
    fj_map_iter_t iter;
    fj_map_iter_init(sys->event_bindings, &iter);
    while (1) {
        fj_map_element_t * elem = fj_map_iter_next(&iter);

        if (elem == NULL) {
            break;
        }

        fj_idlist_t * bindings = elem->value;
        fj_idlist_del(bindings);
    }

    fj_map_del(sys->service_interfaces);
    sys->service_interfaces = NULL;
}


static void sys_destroy(fj_sys_t * sys)
{
    if (sys->service_interfaces != NULL) {
        sys_destroy_service_interfaces(sys);
    }

    if (sys->entity_data != NULL) {
        sys_destroy_entity_data(sys);
    }

    if (sys->event_bindings != NULL) {
        sys_destroy_event_bindings(sys);
    }
}


fj_sys_t * fj_sys_new()
{
    fj_sys_t * sys = calloc(1, sizeof(fj_sys_t));

    if (sys == NULL) {
        return NULL;
    }

    if (sys_init(sys) != FJ_OK) {
        fj_sys_del(sys);
        return NULL;
    }

    return sys;
}


void fj_sys_del(fj_sys_t * sys)
{
    sys_destroy(sys);
    free(sys);
}


fj_result_t fj_sys_set_interface(
    fj_sys_t * sys,
    fj_id_t service_id,
    fj_id_t interface_id,
    fj_ptr_t interface_object
);

fj_result_t fj_sys_remove_interface(
    fj_sys_t * sys,
    fj_id_t service_id,
    fj_id_t interface_id
);

fj_ptr_t fj_sys_get_interface(
    fj_sys_t * sys,
    fj_id_t interface_id
);

fj_ptr_t fj_sys_get_interface_of_service(
    fj_sys_t * sys,
    fj_id_t service_id,
    fj_id_t interface_id
);

fj_result_t fj_sys_set_entity_data(
    fj_sys_t * sys,
    fj_id_t service_id,
    fj_id_t entity_id,
    fj_ptr_t data
);

fj_result_t fj_sys_remove_entity_data(
    fj_sys_t * sys,
    fj_id_t service_id,
    fj_id_t entity_id,
    fj_ptr_t data
);

fj_ptr_t fj_sys_get_entity_data(
    fj_sys_t * sys,
    fj_id_t service_id,
    fj_id_t entity_id
);

fj_result_t fj_sys_bind_event(
    fj_sys_t * sys,
    fj_id_t sending_entity_id,
    fj_id_t sent_event_id,
    fj_id_t receiving_service_id
);