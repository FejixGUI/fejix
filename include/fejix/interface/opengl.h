#ifndef FEJIX_INTERFACE_OPENGL_H_
#define FEJIX_INTERFACE_OPENGL_H_


#include <fejix/interface/client.h>
#include <fejix/interface/window.h>


/* Based on khrplatform.h from Khronos. */
#if defined(_WIN32) && !defined(_WIN32_WCE) && !defined(__SCITECH_SNAP__)
#    define FJ_OPENGL_ABI_IMPL __stdcall
#else
#    define FJ_OPENGL_ABI_IMPL
#endif

/** OpenGL ABI calling convention native to the target OS. */
#define FJ_OPENGL_ABI FJ_OPENGL_ABI_IMPL


typedef uint32_t fj_opengl_implementation_id_t;

enum fj_opengl_implementation_id {
    /** Khronos native platform graphics interface. */
    FJ_OPENGL_IMPLEMENTATION_EGL,

    /** Windows OpenGL interface. */
    FJ_OPENGL_IMPLEMENTATION_WGL,

    /** OpenGL over X interface. */
    FJ_OPENGL_IMPLEMENTATION_GLX,
};


typedef uint32_t fj_opengl_feature_id_t;

enum fj_opengl_feature_id {
    FJ_OPENGL_FEATURE_GLOBAL_SAME_FUNCTION_GETTER,
    FJ_OPENGL_FEATURE_GLOBAL_PROTECTED_MODE,
    FJ_OPENGL_FEATURE_MANAGER_SAME_FUNCTION_GETTER,
    FJ_OPENGL_FEATURE_CONTEXT_SAME_FUNCTION_GETTER,
    FJ_OPENGL_FEATURE_CONTEXT_API_OPENGL_DESKTOP,
    FJ_OPENGL_FEATURE_CONTEXT_API_OPENGL_ES,
    FJ_OPENGL_FEATURE_CONTEXT_SHARING,
    FJ_OPENGL_FEATURE_CONTEXT_SEPARATE_READ_WRITE,
    FJ_OPENGL_FEATURE_CANVAS_COLORSPACE_SRGB,
    FJ_OPENGL_FEATURE_CANVAS_MULTISAMPLING,
};


typedef int32_t fj_opengl_int_t;

enum fj_opengl_canvas_attribute_id {
    FJ_OPENGL_CANVAS_ATTRIBUTE_END,
    FJ_OPENGL_CANVAS_ATTRIBUTE_BUFFER_WIDTH,
    FJ_OPENGL_CANVAS_ATTRIBUTE_BUFFER_HEIGHT,
    FJ_OPENGL_CANVAS_ATTRIBUTE_BUFFER_COUNT,
    FJ_OPENGL_CANVAS_ATTRIBUTE_BUFFER_STEREO,
    FJ_OPENGL_CANVAS_ATTRIBUTE_MULTISAMPLING_BUFFER_COUNT,
    FJ_OPENGL_CANVAS_ATTRIBUTE_MULTISAMPLING_SAMPLE_COUNT,
    FJ_OPENGL_CANVAS_ATTRIBUTE_RED_BITS,
    FJ_OPENGL_CANVAS_ATTRIBUTE_GREEN_BITS,
    FJ_OPENGL_CANVAS_ATTRIBUTE_BLUE_BITS,
    FJ_OPENGL_CANVAS_ATTRIBUTE_ALPHA_BITS,
    FJ_OPENGL_CANVAS_ATTRIBUTE_ACCUM_RED_BITS,
    FJ_OPENGL_CANVAS_ATTRIBUTE_ACCUM_GREEN_BITS,
    FJ_OPENGL_CANVAS_ATTRIBUTE_ACCUM_BLUE_BITS,
    FJ_OPENGL_CANVAS_ATTRIBUTE_ACCUM_ALPHA_BITS,
    FJ_OPENGL_CANVAS_ATTRIBUTE_DEPTH_BITS,
    FJ_OPENGL_CANVAS_ATTRIBUTE_STENCIL_BITS,
    FJ_OPENGL_CANVAS_ATTRIBUTE_COLORSPACE_SRGB,
    FJ_OPENGL_CANVAS_ATTRIBUTE_PROTECTED,
    // TODO
};

enum fj_opengl_context_attribute_id {
    FJ_OPENGL_CONTEXT_ATTRIBUTE_END,
    FJ_OPENGL_CONTEXT_ATTRIBUTE_API_OPENGL_DESKTOP,
    FJ_OPENGL_CONTEXT_ATTRIBUTE_API_OPENGL_ES,
    FJ_OPENGL_CONTEXT_ATTRIBUTE_API_VERSION_MAJOR,
    FJ_OPENGL_CONTEXT_ATTRIBUTE_API_VERSION_MINOR,
    FJ_OPENGL_CONTEXT_ATTRIBUTE_API_BACKWARD_COMPATIBLE,
    FJ_OPENGL_CONTEXT_ATTRIBUTE_API_FOREWARD_COMPATIBLE,
    FJ_OPENGL_CONTEXT_ATTRIBUTE_DEBUG,
    FJ_OPENGL_CONTEXT_ATTRIBUTE_PROTECTED,
    FJ_OPENGL_CONTEXT_ATTRIBUTE_INDIRECT,
    // TODO
};


typedef void(FJ_OPENGL_ABI *fj_opengl_function_t)(void);

typedef fj_opengl_function_t(FJ_OPENGL_ABI *fj_opengl_function_getter_t)(char const *function_name);


struct fj_opengl_manager;
struct fj_opengl_canvas_builder;
struct fj_opengl_canvas;
struct fj_opengl_context;

struct fj_opengl_native_state {
    void *native_context;
    void *native_read_canvas;
    void *native_write_canvas;
};

struct fj_opengl_manager_create_info {
    /** If NULL, the default function getter will be used. */
    fj_opengl_function_getter_t imported_function_getter;

    /**
        If not NULL, imports the native manager from this field.

        Accepts EGLDisplay* for EGL.
    */
    void const *imported_native_manager;
};

struct fj_opengl_canvas_builder_create_info {
    /** Array of the form { ATTRIBUTE_ID, VALUE, ATTRIBUTE_ID, VALUE, ATTRIBUTE_END } */
    fj_opengl_int_t const *attributes;
};

struct fj_opengl_context_create_info {
    /** Array of the form { ATTRIBUTE_ID, VALUE, ATTRIBUTE_ID, VALUE, ATTRIBUTE_END } */
    fj_opengl_int_t const *attributes;

    /** Specifies the context with which the new context will share some object IDs. */
    struct fj_opengl_context *share_context;
};


struct fj_opengl_implementation {
    fj_opengl_implementation_id_t id;

    fj_err_t (*create_manager)(
        struct fj_client *client,
        struct fj_opengl_manager **manager,
        struct fj_opengl_manager_create_info const *create_info
    );

    fj_err_t (*destroy_manager)(struct fj_opengl_manager *manager);

    /**
        Returns EGLDisplay* for EGL.
        Returns Display* for GLX.
        Returns NULL for other implementations.
    */
    void (*export_manager)(struct fj_opengl_manager *manager, void **native_manager);

    fj_bool8_t (*get_manager_feature_supported)(
        struct fj_opengl_manager *manager,
        fj_opengl_feature_id_t feature
    );

    fj_bool8_t (*get_manager_extension_supported)(
        struct fj_opengl_manager *manager,
        char const *extension_name
    );

    fj_opengl_function_getter_t (*get_manager_function_getter)(struct fj_opengl_manager *manager);

    /**
        :param window_builder: Can be NULL.
    */
    fj_err_t (*create_canvas_builder)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_canvas_builder **canvas_builder,
        struct fj_opengl_canvas_builder_create_info const *create_info,
        struct fj_window_builder *window_builder
    );

    fj_err_t (*destroy_canvas_builder)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_canvas_builder *canvas_builder
    );

    /**
        :param window: Can be NULL.
    */
    fj_err_t (*create_canvas)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_canvas **canvas,
        struct fj_opengl_canvas_builder *canvas_builder,
        struct fj_window *window
    );

    /**
        :param window: Can be NULL.
    */
    fj_err_t (*destroy_canvas)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_canvas *canvas,
        struct fj_window *window
    );

    /**
        :param native_canvas:
            * returns EGLSurface* for EGL.
            * returns HDC* for WGL.
            * returns GLXDrawable* for GLX.

        :param native_canvas_config:
            * returns EGLConfig* for EGL.
            * returns int32_t* for WGL.
            * returns GLXFBConfig* for GLX.
    */
    void (*export_canvas)(
        struct fj_opengl_canvas *canvas,
        void **native_canvas,
        void **native_canvas_config
    );

    fj_err_t (*present_canvas)(struct fj_opengl_manager *manager, struct fj_opengl_canvas *canvas);

    fj_err_t (*create_context)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_context **context,
        struct fj_opengl_context_create_info const *create_info,
        struct fj_opengl_canvas *canvas
    );

    fj_err_t (*destroy_context)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_context *context
    );

    /**
        :param native_context:
            * returns EGLContext* for EGL.
            * returns HGLRC* for WGL.
            * returns GLXContext* for GLX.
    */
    void (*export_context)(struct fj_opengl_context *context, void **native_context);

    fj_opengl_function_getter_t (*get_context_function_getter)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_context *context
    );

    /** :param context: Set to NULL to make no context current. */
    fj_err_t (*set_current_state)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_context *context,
        struct fj_opengl_canvas *read_canvas,
        struct fj_opengl_canvas *write_canvas
    );

    fj_err_t (*export_current_state)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_native_state *state
    );

    fj_err_t (*import_current_state)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_native_state const *state
    );
};

struct fj_opengl_interface {
    void (*get_implementations)(
        struct fj_opengl_implementation const *const *implementations,
        uint32_t implementation_count
    );
};


#endif
