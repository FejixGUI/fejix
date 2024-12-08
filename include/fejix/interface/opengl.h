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

typedef int32_t fj_opengl_constant_t;

enum fj_opengl_constant {
    FJ_OPENGL_ATTRIBUTE_LIST_END,
    FJ_OPENGL_CONTEXT_API_OPENGL_DESKTOP,
    FJ_OPENGL_CONTEXT_API_OPENGL_ES,
    FJ_OPENGL_CONTEXT_API_VERSION_MAJOR,
    FJ_OPENGL_CONTEXT_API_VERSION_MINOR,
    FJ_OPENGL_CONTEXT_API_BACKWARD_COMPATIBLE,
    FJ_OPENGL_CONTEXT_API_FOREWARD_COMPATIBLE,
    FJ_OPENGL_CONTEXT_PROTECTED,
    FJ_OPENGL_CANVAS_PROTECTED,
    FJ_OPENGL_CANVAS_BUFFER_COUNT,
    FJ_OPENGL_CANVAS_BUFFER_STEREO,
    FJ_OPENGL_CANVAS_MULTISAMPLING_BUFFER_COUNT,
    FJ_OPENGL_CANVAS_MULTISAMPLING_SAMPLE_COUNT,
    FJ_OPENGL_CANVAS_RED_BITS,
    FJ_OPENGL_CANVAS_GREEN_BITS,
    FJ_OPENGL_CANVAS_BLUE_BITS,
    FJ_OPENGL_CANVAS_ALPHA_BITS,
    FJ_OPENGL_CANVAS_ACCUMULATION_RED_BITS,
    FJ_OPENGL_CANVAS_ACCUMULATION_GREEN_BITS,
    FJ_OPENGL_CANVAS_ACCUMULATION_BLUE_BITS,
    FJ_OPENGL_CANVAS_ACCUMULATION_ALPHA_BITS,
    FJ_OPENGL_CANVAS_DEPTH_BITS,
    FJ_OPENGL_CANVAS_STENCIL_BITS,
    FJ_OPENGL_CANVAS_COLORSPACE_SRGB,
    // TODO
};


typedef void (*fj_opengl_function_t)(void);

typedef fj_opengl_function_t (*fj_opengl_function_getter_t)(char const *function_name);


struct fj_opengl_manager;
struct fj_opengl_canvas_builder;
struct fj_opengl_canvas;
struct fj_opengl_context;


struct fj_opengl_manager_create_info {
    /** If NULL, the default function getter will be used. */
    fj_opengl_function_getter_t function_getter;

    /**
        Can be EGLDisplay* for EGL.
        Must be NULL if unused.
    */
    void const *extra_info;
};

struct fj_opengl_canvas_create_info {
    fj_opengl_constant_t *attributes;
};

struct fj_opengl_context_create_info {
    fj_opengl_constant_t *attributes;

    /** Specifies the context with which the new context will share some object IDs. */
    struct fj_opengl_context *share_context;
};


struct fj_opengl_implementation {
    fj_opengl_implementation_id_t id;

    fj_err_t (*create_manager)(
        struct fj_opengl_manager **manager,
        struct fj_client *client,
        struct fj_opengl_manager_create_info const *create_info
    );

    fj_err_t (*destroy_manager)(struct fj_opengl_manager *manager);

    /**
        Fejix virtual extensions:
        * FEJIX_global_function_getter
        * FEJIX_global_same_function_getter
        * FEJIX_global_protected_mode
        * FEJIX_manager_same_function_getter
        * FEJIX_context_same_function_getter
        * FEJIX_context_api_opengl_desktop
        * FEJIX_context_api_opengl_es
        * FEJIX_context_sharing
        * FEJIX_context_separate_read_write
        * FEJIX_canvas_colorspace_srgb
        * FEJIX_canvas_multisampling
    */
    fj_bool8_t (*get_manager_extension_supported)(
        struct fj_opengl_manager *manager,
        char const *extension_name
    );

    fj_opengl_function_t (*get_manager_function)(
        struct fj_opengl_manager *manager,
        char const *function_name
    );

    /**
        Returns EGLDisplay* for EGL.
        Returns Display* for GLX.
        Returns NULL for other implementations.
    */
    void *(*export_manager)(struct fj_opengl_manager *manager);

    /**
        :param window_builder: Can be NULL.
    */
    fj_err_t (*create_canvas_builder)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_canvas_builder **canvas_builder,
        struct fj_window_builder *window_builder,
        struct fj_opengl_canvas_create_info const *create_info
    );

    fj_err_t (*destroy_canvas_builder)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_canvas_builder *canvas_builder
    );

    fj_err_t (*get_canvas_builder_attribute)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_canvas_builder *canvas_builder,
        fj_opengl_constant_t attribute,
        fj_opengl_constant_t *value
    );

    /**
        Returns EGLConfig* for EGL.
        Returns uint32_t* (pixel format) for WGL.
        Returns GLXFBConfig* for GLX.
    */
    void *(*export_canvas_builder)(struct fj_opengl_canvas_builder *canvas_builder);

    /**
        :param window: Can be NULL.
    */
    fj_err_t (*create_canvas)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_canvas **canvas,
        struct fj_opengl_canvas_builder *canvas_builder,
        struct fj_window *window
    );

    // TODO do we need window when destroying canvas?
    /**
        :param window: Can be NULL.
    */
    fj_err_t (*destroy_canvas)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_canvas *canvas,
        struct fj_window *window
    );

    fj_err_t (*present_canvas)(struct fj_opengl_manager *manager, struct fj_opengl_canvas *canvas);

    /**
        Returns EGLSurface* for EGL.
        Returns HDC* for WGL.
        Returns GLXWindow* for GLX.
    */
    void *(*export_canvas)(struct fj_opengl_canvas *canvas);

    fj_err_t (*create_context)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_context **context,
        struct fj_opengl_canvas *canvas,
        struct fj_opengl_context_create_info const *create_info
    );

    fj_err_t (*destroy_context)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_context *context
    );

    /** :param context: Set to NULL to make no context current. */
    fj_err_t (*set_context_current)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_context *context,
        struct fj_opengl_canvas *read_canvas,
        struct fj_opengl_canvas *write_canvas
    );

    fj_opengl_function_getter_t (*get_context_function_getter)(struct fj_opengl_manager *manager);

    /**
        Returns EGLContext* for EGL.
        Returns HGLRC* for WGL.
        Returns GLXContext* for GLX.
    */
    void *(*export_context)(struct fj_opengl_context *context);
};

struct fj_opengl_interface {
    void (*get_implementations)(
        struct fj_opengl_implementation const *const *implementations,
        uint32_t implementation_count
    );
};


#endif
