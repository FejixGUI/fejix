#ifndef FEJIX_SYS_H_
#define FEJIX_SYS_H_


#include <fejix/core/base.h>


#define FJ_INTERFACE(INTERFACE) INTERFACE

#define FJ_METHOD(METHOD) (*METHOD)

#define FJ_EVENT(EVENT) EVENT

#define FJ_IMPL_BEGIN(INTERFACE, INSTANCE) \
    static struct INTERFACE INSTANCE = {

#define FJ_IMPL_END() };

#define FJ_IMPL(METHOD, IMPLEMENTATION) \
    .METHOD = IMPLEMENTATION,

#define FJ_IMPL_EVENT_HANDLER(INSTANCE, CALLBACK) \
    FJ_IMPL_BEGIN(fj_event_handler, INSTANCE) \
        FJ_IMPL(handle_event, CALLBACK) \
    FJ_IMPL_END()


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


struct FJ_INTERFACE(fj_event_handler) {
    fj_err_t FJ_METHOD(handle_event)(
        struct fj_sys * sys,
        struct fj_event_data * event_data
    );
};


/// Returns NULL if an allocation error occurs.
struct fj_sys * fj_sys_new(void);

void fj_sys_del(struct fj_sys * sys);

/// Remove the interface by setting it to NULL.
/// However, removing interfaces is potentially dengerous.
/// See `fj_sys_emit_event()`.
fj_err_t fj_sys_set_interface(
    struct fj_sys * sys,
    fj_id_t interface_id,
    fj_ptr_t interface
);

fj_ptr_t fj_sys_get_interface(
    struct fj_sys * sys,
    fj_id_t interface_id
);

/// Remove the resource by setting it to NULL.
/// Resources are not deallocated by this function.
fj_err_t fj_sys_set_resource(
    struct fj_sys * sys,
    fj_id_t entity_id,
    fj_id_t interface_id,
    fj_ptr_t resource
);

fj_ptr_t fj_sys_get_resource(
    struct fj_sys * sys,
    fj_id_t entity_id,
    fj_id_t interface_id
);

fj_err_t fj_sys_bind_event(
    struct fj_sys * sys,
    fj_id_t entity_id,
    fj_id_t event_id,
    fj_id_t handler_interface_id
);

fj_err_t fj_sys_unbind_event(
    struct fj_sys * sys,
    fj_id_t entity_id,
    fj_id_t event_id,
    fj_id_t handler_interface_id
);

/// It is safe to modify `sys` in the event handlers.
///
/// If a handler interface does not exist, then an error is returned.
fj_err_t fj_sys_emit_event(
    struct fj_sys * sys,
    fj_id_t entity_id,
    fj_id_t event_id,
    fj_ptr_t event
);


#endif
