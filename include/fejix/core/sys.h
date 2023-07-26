#ifndef FEJIX_SYS_H_
#define FEJIX_SYS_H_


#include <fejix/core/list.h>
#include <fejix/core/map.h>


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


struct fj_sys;


struct fj_event_data {
    fj_ptr_t event;
    fj_id_t event_id;
    fj_id_t entity_id;
};

typedef fj_result_t (*fj_event_handler_t)(
    struct fj_sys * sys,
    struct fj_event_data * event_data
);

struct fj_event_handler_interface {
    fj_event_handler_t handle_event;
};


struct fj_sys * fj_sys_new(void);

void fj_sys_del(struct fj_sys * sys);

/// Remove the interface by setting it to NULL
fj_result_t fj_sys_set_interface(
    struct fj_sys * sys,
    fj_id_t module_id,
    fj_id_t interface_id,
    fj_ptr_t interface
);

fj_ptr_t fj_sys_get_interface(
    struct fj_sys * sys,
    fj_id_t module_id,
    fj_id_t interface_id
);

fj_ptr_t fj_sys_find_interface(
    struct fj_sys * sys,
    fj_id_t interface_id
);

/// Remove resource by setting it to NULL
fj_result_t fj_sys_set_resource(
    struct fj_sys * sys,
    fj_id_t module_id,
    fj_id_t entity_id,
    fj_ptr_t resource
);

fj_ptr_t fj_sys_get_resource(
    struct fj_sys * sys,
    fj_id_t module_id,
    fj_id_t entity_id
);

fj_result_t fj_sys_bind_event(
    struct fj_sys * sys,
    fj_id_t entity_id,
    fj_id_t event_id,
    fj_id_t handler_module_id
);

fj_result_t fj_sys_unbind_event(
    struct fj_sys * sys,
    fj_id_t entity_id,
    fj_id_t event_id,
    fj_id_t handler_module_id
);

fj_result_t fj_sys_emit_event(
    struct fj_sys * sys,
    fj_id_t entity_id,
    fj_id_t event_id,
    fj_ptr_t event
);


#endif