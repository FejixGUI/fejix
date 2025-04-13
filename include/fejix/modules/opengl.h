#ifndef FEJIX_OPENGL_H_INCLUDED
#define FEJIX_OPENGL_H_INCLUDED


#include <fejix/modules/app.h>


typedef void *(*fj_opengl_function_load_callback)(void *callback_data, char const *function_name);


struct fj_opengl_manager_info {
    void *egl_display;
    fj_opengl_function_load_callback function_load_callback;
    void *function_load_callback_data;
};

struct fj_opengl_window_surface_info {
    void *egl_config;
    intptr_t const *egl_attribs;
};


FJ_OPAQUE_STRUCT_WITH_USERDATA(fj_opengl_manager)

FJ_METHOD_NONNULL(
    fj_opengl_create_manager,
    fj_err,
    struct fj_app_manager *app_manager,
    struct fj_opengl_manager_info const *manager_info,
    struct fj_opengl_manager **out_manager)

FJ_METHOD(fj_opengl_destroy_manager, fj_err, struct fj_opengl_manager *manager)

FJ_METHOD(
    fj_opengl_create_window_surface,
    fj_err,
    struct fj_opengl_manager *manager,
    struct fj_opengl_window_surface_info const *surface_info,
    void **out_egl_surface)

FJ_METHOD_LIST_BEGIN(fj_opengl)
FJ_METHOD_LIST_ITEM(fj_opengl_create_manager)
FJ_METHOD_LIST_ITEM(fj_opengl_destroy_manager)
FJ_METHOD_LIST_ITEM(fj_opengl_create_surface)
FJ_METHOD_LIST_END()

#endif
