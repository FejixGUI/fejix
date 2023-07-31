#ifndef FEJIX_SYS_H_
#define FEJIX_SYS_H_


#include <fejix/core/base.h>


#define FJ_DEFINE_INTERFACE(INTERFACE_NAME) \
    struct INTERFACE_NAME

#define FJ_DEFINE_METHOD(METHOD_NAME, RETURN_TYPE, ARGS) \
    RETURN_TYPE (*METHOD_NAME) ARGS;

#define FJ_IMPLEMENT_INTERFACE(INTERFACE_NAME, INSTANCE_NAME) \
    static struct INTERFACE_NAME INSTANCE_NAME =

#define FJ_IMPLEMENT_METHOD(METHOD_NAME, IMPLEMENTATION_NAME) \
    .METHOD_NAME = IMPLEMENTATION_NAME,

#define FJ_IMPLEMENT_EVENT_INTERFACE(INSTANCE_NAME, CALLBACK_NAME) \
    FJ_IMPLEMENT_INTERFACE(fj_event_handler_interface, INSTANCE_NAME) { \
        FJ_IMPLEMENT_METHOD(handle_event, CALLBACK_NAME) \
    };


enum fj_sys_standard_entities {
    FJ_GLOBAL_ENTITY = 0,
};

enum fj_sys_standard_events {
    FJ_GLOBAL_SETUP = 0,
    FJ_GLOBAL_SHUTDOWN = 1,
    FJ_ENTITY_CREATE = 2,
    FJ_ENTITY_DESTROY = 3,
};


struct fj_sys;


struct fj_event_data {
    fj_ptr_t event;
    fj_id_t event_id;
    fj_id_t entity_id;
};


FJ_DEFINE_INTERFACE(fj_event_handler_interface) {
    FJ_DEFINE_METHOD(handle_event, fj_err_t, (
        struct fj_sys * sys,
        struct fj_event_data * event_data
    ))
};


/// Returns NULL if `fj_err_malloc` occurs.
struct fj_sys * fj_sys_new(void);

void fj_sys_del(struct fj_sys * sys);

/// Remove the interface by setting it to NULL.
/// However, removing interfaces is potentially dengerous.
/// See `fj_sys_emit_event()`.
fj_err_t fj_sys_set_interface(
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

/// Finds the module that provides an interface with the given ID and returns
///the interface. This is supposed to work for globally unique interfaces.
/// Returns NULL if the interface cannot be found.
fj_ptr_t fj_sys_find_interface(
    struct fj_sys * sys,
    fj_id_t interface_id
);

/// Remove the resource by setting it to NULL
fj_err_t fj_sys_set_resource(
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

fj_err_t fj_sys_bind_event(
    struct fj_sys * sys,
    fj_id_t handler_module_id,
    fj_id_t entity_id,
    fj_id_t event_id
);

fj_err_t fj_sys_unbind_event(
    struct fj_sys * sys,
    fj_id_t handler_module_id,
    fj_id_t entity_id,
    fj_id_t event_id
);

/// It is safe to modify `sys` in the event handlers.
///
/// If a handler module does not have the appropriate event handling interface
/// (such interface should be of type `struct fj_event_handler_interface` and
/// have the same ID as the event), then an error is returned.
fj_err_t fj_sys_emit_event(
    struct fj_sys * sys,
    fj_id_t entity_id,
    fj_id_t event_id,
    fj_ptr_t event
);


#endif
