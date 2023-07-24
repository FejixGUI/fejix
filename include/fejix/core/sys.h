#ifndef FEJIX_SYS_H_
#define FEJIX_SYS_H_


#include <fejix/core/idlist.h>
#include <fejix/core/map.h>


struct fj_sys;


typedef struct fj_sys fj_sys_t;


enum fj_sys_standard_entities {
    FJ_SYS_GLOBAL_ENTITY = 0,
    FJ_SYS_DEFAULT_ENTITY = 1,
};

enum fj_sys_standard_events {
    FJ_SYS_GLOBAL_SETUP = 0,
    FJ_SYS_GLOBAL_SHUTDOWN = 1,
    FJ_SYS_ENTITY_CREATE = 2,
    FJ_SYS_ENTITY_DESTROY = 3,
};


struct fj_sys {
    /// module_id -> interface_id -> *interface
    fj_map_t * module_interfaces;

    /// module_id -> entity_id -> *resource
    fj_map_t * module_resources;

    /// entity_id -> event_id -> modules_that_handle_the_event[]
    fj_map_t * event_bindings;
};


fj_sys_t * fj_sys_new(void);

void fj_sys_del(fj_sys_t * sys);

fj_result_t fj_sys_add_module(fj_sys_t * sys, fj_id_t module_id);

fj_result_t fj_sys_remove_module(fj_sys_t * sys, fj_id_t module_id);

fj_result_t fj_sys_add_interface(
    fj_sys_t * sys,
    fj_id_t module_id,
    fj_id_t interface_id,
    fj_ptr_t interface
);

fj_result_t fj_sys_remove_interface(
    fj_sys_t * sys,
    fj_id_t module_id,
    fj_id_t interface_id
);

fj_ptr_t fj_sys_get_interface(
    fj_sys_t * sys,
    fj_id_t interface_id
);

fj_ptr_t fj_sys_get_module_interface(
    fj_sys_t * sys,
    fj_id_t module_id,
    fj_id_t interface_id
);

fj_result_t fj_sys_add_resource(
    fj_sys_t * sys,
    fj_id_t module_id,
    fj_id_t entity_id,
    fj_ptr_t resource
);

fj_result_t fj_sys_remove_resource(
    fj_sys_t * sys,
    fj_id_t module_id,
    fj_id_t entity_id
);

fj_ptr_t fj_sys_get_resource(
    fj_sys_t * sys,
    fj_id_t module_id,
    fj_id_t entity_id
);

fj_result_t fj_sys_bind_event(
    fj_sys_t * sys,
    fj_id_t entity_id,
    fj_id_t event_id,
    fj_id_t handler_module_id
);

fj_result_t fj_sys_unbind_event(
    fj_sys_t * sys,
    fj_id_t entity_id,
    fj_id_t event_id,
    fj_id_t module_id
);

fj_result_t fj_sys_emit_event(
    fj_sys_t * sys,
    fj_id_t entity_id,
    fj_id_t event_id,
    fj_ptr_t event
);


#endif