#include <fejix/core/sys.h>

#include <malloc.h>


fj_sys_t * fj_sys_new()
{
    
}

void fj_sys_del(fj_sys_t * sys);

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