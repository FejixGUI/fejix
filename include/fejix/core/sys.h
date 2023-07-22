#ifndef FEJIX_SYS_H_
#define FEJIX_SYS_H_


#include <fejix/core/idlist.h>
#include <fejix/core/map.h>


struct fj_sys;


typedef struct fj_sys fj_sys_t;


enum fj_sys_standard_entity_ids {
    FJ_GLOBAL_ENTITY = 0,
    FJ_DEFAULT_ENTITY = 1,
};


struct fj_sys {
    /// service_id -> interface_id -> *interface_object
    fj_map_t * service_interfaces;

    /// entity_id -> event_id -> receiving_service_ids[]
    fj_map_t * event_bindings;

    /// entity_id -> service_id -> *data
    fj_map_t * entity_data;
};


fj_sys_t * fj_sys_new();

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

fj_result_t fj_sys_unbind_event(
    fj_sys_t * sys,
    fj_id_t sending_entity_id,
    fj_id_t sent_event_id,
    fj_id_t receiving_service_id
);

fj_result_t fj_sys_emit_event(
    fj_sys_t * sys,
    fj_id_t sending_entity_id,
    fj_id_t sent_event_id,
    fj_ptr_t sent_event_data
);


#endif