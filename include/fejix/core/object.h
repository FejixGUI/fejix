#ifndef FEJIX_OBJECT_H_
#define FEJIX_OBJECT_H_


#include <fejix/core/base.h>


struct fj_object;


struct fj_object * fj_object_new(void);

void fj_object_del(struct fj_object * obj);

/* Remove the property by setting it to NULL. */
fj_err_t fj_object_set_property(
    struct fj_object * object,
    fj_id_t property_id,
    fj_ptr_t property_value
);

fj_ptr_t fj_object_get_property(
    struct fj_object * object,
    fj_id_t property_id
);

/* Unbind the listener by binding it to NULL. */
fj_err_t fj_object_set_listener(
    struct fj_object * object,
    fj_id_t listener_id,
    fj_ptr_t listener_interface
);

fj_ptr_t fj_object_get_listener(
    struct fj_object * object,
    fj_id_t listener_id
);


#endif
