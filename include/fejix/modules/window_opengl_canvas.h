/**
    \file
*/

#ifndef FEJIX_WINDOW_OPENGL_CANVAS_H_INCLUDED
#define FEJIX_WINDOW_OPENGL_CANVAS_H_INCLUDED


#include <fejix/modules/window.h>


FJ_OBJECT_TYPE(fj_window_opengl_canvas_manager)
FJ_OBJECT_TYPE(fj_window_opengl_canvas)


struct fj_window_opengl_canvas_info {
    void *egl_display;
    void *egl_config;
    intptr_t const *egl_attribs;
    void (*(*function_load_callback)(void *callback_data, char const *name))(void);
    void *function_load_callback_data;
};


FJ_METHOD_WITH_FALLBACK(
    fj_window_opengl_canvas_create_manager,
    enum fj_error,
    FJ_ERROR_UNIMPLEMENTED,
    struct fj_io_thread *io_thread,
    struct fj_window_opengl_canvas_manager **out_manager)

FJ_METHOD(
    fj_window_opengl_destroy_manager,
    enum fj_error,
    struct fj_window_opengl_canvas_manager *manager)

FJ_METHOD(
    fj_window_opengl_canvas_create,
    enum fj_error,
    struct fj_window_opengl_canvas_manager *manager,
    struct fj_window *window,
    struct fj_window_opengl_canvas_info const *canvas_info,
    struct fj_window_opengl_canvas **out_canvas)

FJ_METHOD(
    fj_window_opengl_canvas_destroy,
    enum fj_error,
    struct fj_window_opengl_canvas_manager *manager,
    struct fj_window_opengl_canvas *canvas)

/**
    \returns `EGLSurface`, NULL if the surface has not been created yet,
        e.g. if the surface is not synced yet.
*/
FJ_METHOD(fj_window_opengl_canvas_get_surface, void *, struct fj_window_opengl_canvas *canvas)


FJ_METHOD_LIST_BEGIN(fj_window_opengl_canvas)
FJ_METHOD_LIST_ITEM(fj_window_opengl_canvas_create_manager)
FJ_METHOD_LIST_ITEM(fj_window_opengl_canvas_destroy_manager)
FJ_METHOD_LIST_ITEM(fj_window_opengl_canvas_create)
FJ_METHOD_LIST_ITEM(fj_window_opengl_canvas_destroy)
FJ_METHOD_LIST_ITEM(fj_window_opengl_canvas_get_surface)
FJ_METHOD_LIST_END()


#endif
