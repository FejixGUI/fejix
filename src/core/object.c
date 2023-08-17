#include <fejix/core/object.h>
#include <fejix/core/utils.h>
#include <fejix/core/malloc.h>
#include <fejix/core/map.h>


struct fj_object {
    struct fj_map * properties;
    struct fj_map * listeners;
};


struct fj_object * fj_object_new(void)
{
    struct fj_object * obj = fj_alloc_zeroed(sizeof(struct fj_object));
    if (obj == NULL) {
        return NULL;
    }

    obj->properties = fj_map_new();
    if (obj->properties == NULL) {
        fj_object_del(obj);
        return NULL;
    }

    obj->listeners = fj_map_new();
    if (obj->listeners == NULL) {
        fj_object_del(obj);
        return NULL;
    }

    return obj;
}


void fj_object_del(struct fj_object * obj)
{
    if (obj->properties != NULL) {
        fj_map_del(obj->properties);
    }

    if (obj->listeners != NULL) {
        fj_map_del(obj->listeners);
    }

    fj_free(obj);
}

fj_err_t fj_object_set_property(
    struct fj_object * object,
    fj_id_t property_id,
    fj_ptr_t property_value
)
{
    return fj_map_set(object->properties, property_id, property_value);
}


fj_ptr_t fj_object_get_property(
    struct fj_object * object,
    fj_id_t property_id
)
{
    return fj_map_get(object->properties, property_id);
}


fj_err_t fj_object_set_listener(
    struct fj_object * object,
    fj_id_t listener_id,
    fj_ptr_t listener_interface
)
{
    return fj_map_set(object->listeners, listener_id, listener_interface);
}


fj_ptr_t fj_object_get_listener(
    struct fj_object * object,
    fj_id_t listener_id
)
{
    return fj_map_get(object->listeners, listener_id);
}
