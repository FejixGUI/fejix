#ifndef FEJIX_INTERFACE_OPENGL_H_
#define FEJIX_INTERFACE_OPENGL_H_


#include <fejix/interface/client.h>
#include <fejix/interface/window.h>


typedef uint32_t fj_opengl_implementation_id_t;

enum fj_opengl_implementation_id {
    /** Khronos native platform graphics interface. */
    FJ_OPENGL_IMPLEMENTATION_EGL,

    /** Windows OpenGL interface. */
    FJ_OPENGL_IMPLEMENTATION_WGL,

    /** OpenGL over X interface. */
    FJ_OPENGL_IMPLEMENTATION_GLX,
};


struct fj_opengl_manager;
struct fj_opengl_canvas_builder;
struct fj_opengl_canvas;


struct fj_opengl_manager_create_info {
    void const *(*get_function)(void *callback_data, char const *function_name);
    void *get_function_callback_data;

    /**
        Must be EGLDisplay* for EGL.
        Must be NULL for other implementations.
    */
    void const *extra_display;
};

struct fj_opengl_canvas_builder_create_info {
    /**
        Must be EGLConfig* (surface_config) for EGL.
        Must be uint32_t* (pixel_format) for WGL.
        Must be GLXFBConfig* (framebuffer_config) for GLX.
        Must be NULL for other implementations.
    */
    void const *extra_canvas_config;

    /**
        Must be EGLAttrib* (attribute_array) or NULL for EGL.
        Must be NULL for other implementations.
    */
    void const *extra_canvas_attribs;
};


struct fj_opengl_implementation {
    fj_opengl_implementation_id_t id;

    /**
        Returns Display* (x11_display) for GLX.
        Returns NULL for other implementations.
    */
    void *(*get_native_client)(struct fj_client *client);

    /**
        Returns EGLSurface* for EGL.
        Returns HDC* (window_device_context) for WGL.
        Returns uint32_t* (glx_window_id) for GLX.
        Returns NULL for other implementations.
    */
    void *(*get_native_canvas)(struct fj_opengl_canvas *canvas);

    fj_err_t (*create_manager)(
        struct fj_opengl_manager **manager,
        struct fj_client *client,
        struct fj_opengl_manager_create_info const *create_info
    );

    fj_err_t (*destroy_manager)(struct fj_opengl_manager *manager);

    fj_err_t (*create_canvas_builder)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_canvas_builder **canvas_builder,
        struct fj_window_builder *window_builder,
        struct fj_opengl_canvas_builder_create_info const *create_info
    );

    fj_err_t (*destroy_canvas_builder)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_canvas_builder *canvas_builder
    );

    fj_err_t (*create_canvas)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_canvas **canvas,
        struct fj_opengl_canvas_builder *canvas_builder,
        struct fj_window *window
    );

    fj_err_t (*destroy_canvas)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_canvas *canvas,
        struct fj_window *window
    );
};

struct fj_opengl_interface {
    void (*get_implementations)(
        struct fj_opengl_implementation const *const *implementations,
        uint32_t implementation_count
    );
};


#endif
