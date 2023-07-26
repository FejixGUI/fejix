#ifndef FEJIX_SYS_H_
#define FEJIX_SYS_H_


#include <fejix/core/list.h>
#include <fejix/core/map.h>


#define FJ_DEFINE_INTERFACE(INTERFACE_NAME) \
    struct INTERFACE_NAME

#define FJ_DEFINE_METHOD(METHOD_NAME, RETURN_TYPE, ARGS) \
    RETURN_TYPE (*METHOD_NAME) ARGS;

#define FJ_IMPLEMENT_INTERFACE(INTERFACE_NAME, IMPL_NAME) \
    static struct INTERFACE_NAME IMPL_NAME =

#define FJ_IMPLEMENT_METHOD(METHOD_NAME, IMPL_NAME) \
    .METHOD_NAME = IMPL_NAME,

#define FJ_DESCRIBE_MODULE(MODULE_NAME) \
    static struct fj_interface_description MODULE_NAME[] =

#define FJ_DESCRIBE_INTERFACE(INTERFACE_ID, IMPL_NAME) \
    { .interface_id = INTERFACE_ID, .interface = &IMPL_NAME },

#define FJ_DESCRIBE_END \
    { .interface_id = 0, .interface = NULL }


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

struct fj_interface_description {
    fj_ptr_t interface;
    fj_id_t interface_id;
};

FJ_DEFINE_INTERFACE(fj_event_handler_interface) {
    FJ_DEFINE_METHOD(handle_event, fj_result_t, (
        struct fj_sys * sys,
        struct fj_event_data * event_data
    ))
};


struct fj_sys * fj_sys_new(void);

void fj_sys_del(struct fj_sys * sys);

/// Remove the interface by setting it to NULL.
/// However, removing interfaces is potentially dengerous.
/// See `fj_sys_emit_event()`.
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

fj_result_t fj_sys_load_module_description(
    struct fj_sys * sys,
    fj_id_t module_id,
    struct fj_interface_description * interface_descriptions
);

/// Remove the resource by setting it to NULL
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

/// It is safe to modify `sys` in the event handlers.
///
/// If a handler module does not have the appropriate event handling interface
/// (such interface should be of type `struct fj_event_handler_interface` and
/// have the same ID as the event), then `FJ_INTERNAL_FAIL` is returned.
fj_result_t fj_sys_emit_event(
    struct fj_sys * sys,
    fj_id_t entity_id,
    fj_id_t event_id,
    fj_ptr_t event
);


#endif