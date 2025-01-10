#ifndef FEJIX_OPENGL_H_
#define FEJIX_OPENGL_H_


#include <fejix/app.h>
#include <fejix/image_scene.h>


/* Based on khrplatform.h from Khronos. */
#if defined(_WIN32) && !defined(_WIN32_WCE) && !defined(__SCITECH_SNAP__)
#    define FJ_OPENGL_CALL_IMPL __stdcall
#else
#    define FJ_OPENGL_CALL_IMPL
#endif

/** OpenGL ABI calling convention native to the target OS. */
#define FJ_OPENGL_CALL FJ_OPENGL_CALL_IMPL


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
    FJ_OPENGL_FEATURE_RENDERER_SAME_FUNCTION_GETTER,
    FJ_OPENGL_FEATURE_RENDERER_API_OPENGL_DESKTOP,
    FJ_OPENGL_FEATURE_RENDERER_API_OPENGL_ES,
    FJ_OPENGL_FEATURE_RENDERER_FRIEND,
    FJ_OPENGL_FEATURE_RENDERER_SEPARATE_INPUT_OUTPUT,
    FJ_OPENGL_FEATURE_IMAGE_COLORSPACE_SRGB,
    FJ_OPENGL_FEATURE_IMAGE_MULTISAMPLING,
};


typedef int32_t fj_opengl_attribute_t;

enum fj_opengl_image_attribute_id {
    FJ_OPENGL_IMAGE_ATTRIBUTE_END,
    FJ_OPENGL_IMAGE_ATTRIBUTE_BUFFER_WIDTH,
    FJ_OPENGL_IMAGE_ATTRIBUTE_BUFFER_HEIGHT,
    FJ_OPENGL_IMAGE_ATTRIBUTE_BUFFER_COUNT,
    FJ_OPENGL_IMAGE_ATTRIBUTE_BUFFER_STEREO,
    FJ_OPENGL_IMAGE_ATTRIBUTE_MULTISAMPLING_BUFFER_COUNT,
    FJ_OPENGL_IMAGE_ATTRIBUTE_MULTISAMPLING_SAMPLE_COUNT,
    FJ_OPENGL_IMAGE_ATTRIBUTE_RED_BITS,
    FJ_OPENGL_IMAGE_ATTRIBUTE_GREEN_BITS,
    FJ_OPENGL_IMAGE_ATTRIBUTE_BLUE_BITS,
    FJ_OPENGL_IMAGE_ATTRIBUTE_ALPHA_BITS,
    FJ_OPENGL_IMAGE_ATTRIBUTE_ACCUM_RED_BITS,
    FJ_OPENGL_IMAGE_ATTRIBUTE_ACCUM_GREEN_BITS,
    FJ_OPENGL_IMAGE_ATTRIBUTE_ACCUM_BLUE_BITS,
    FJ_OPENGL_IMAGE_ATTRIBUTE_ACCUM_ALPHA_BITS,
    FJ_OPENGL_IMAGE_ATTRIBUTE_DEPTH_BITS,
    FJ_OPENGL_IMAGE_ATTRIBUTE_STENCIL_BITS,
    FJ_OPENGL_IMAGE_ATTRIBUTE_COLORSPACE_SRGB,
    FJ_OPENGL_IMAGE_ATTRIBUTE_PROTECTED,
    // TODO
};

enum fj_opengl_renderer_attribute_id {
    FJ_OPENGL_RENDERER_ATTRIBUTE_END,
    FJ_OPENGL_RENDERER_ATTRIBUTE_API_OPENGL_DESKTOP,
    FJ_OPENGL_RENDERER_ATTRIBUTE_API_OPENGL_ES,
    FJ_OPENGL_RENDERER_ATTRIBUTE_API_VERSION_MAJOR,
    FJ_OPENGL_RENDERER_ATTRIBUTE_API_VERSION_MINOR,
    FJ_OPENGL_RENDERER_ATTRIBUTE_API_BACKWARD_COMPATIBLE,
    FJ_OPENGL_RENDERER_ATTRIBUTE_API_FOREWARD_COMPATIBLE,
    FJ_OPENGL_RENDERER_ATTRIBUTE_DEBUG,
    FJ_OPENGL_RENDERER_ATTRIBUTE_PROTECTED,
    FJ_OPENGL_RENDERER_ATTRIBUTE_INDIRECT,
    // TODO
};


typedef void (FJ_OPENGL_CALL *fj_opengl_function_t)(void);

typedef fj_opengl_function_t (FJ_OPENGL_CALL *fj_opengl_function_getter_t)(
    char const *function_name
);


struct fj_opengl_manager;
struct fj_opengl_renderer;

struct fj_opengl_manager_internal_info {
    fj_opengl_implementation_id_t implementation_id;

    /**
        Contains:
        * ``EGLDisplay*`` for EGL.
        * ``Display*`` for GLX.
        * NULL for other implementations.
    */
    void *internal_manager;

    /**
        The context which must be current when using the internal manager function getter.
    */
    void *internal_manager_rendering_context;

    /**
        The image set which must be current when using the internal manager function getter.
    */
    void *internal_manager_image_set;

    fj_opengl_function_getter_t internal_manager_function_getter;
};

struct fj_opengl_image_set_internal_info {
    /**
        Contains:
        * ``EGLSurface*`` for EGL.
        * ``HDC*`` for WGL.
        * ``GLXDrawable*`` for GLX.
    */
    void *internal_image_set;

    /**
        Contains:
        * ``EGLConfig*`` for EGL.
        * ``int32_t*`` for WGL.
        * ``GLXFBConfig*`` for GLX.
    */
    void *internal_image_config;
};

struct fj_opengl_renderer_internal_info {
    /**
        Contains:
        * ``EGLContext*`` for EGL.
        * ``HGLRC*`` for WGL.
        * ``GLXContext*`` for GLX.
    */
    void *internal_renderer;
};

struct fj_opengl_rendering_internal_info {
    void *internal_renderer;
    void *internal_input_image_set;
    void *internal_output_image_set;
};

struct fj_opengl_manager_create_info {
    fj_opengl_implementation_id_t implementation_id;

    /** If NULL, ignored. If any fields are NULL, the default values are used. */
    struct fj_opengl_manager_internal_info const *import_internal_info;
};

struct fj_opengl_image_create_info {
    /** Array of the form { ATTRIBUTE_ID, VALUE, ATTRIBUTE_ID, VALUE, ATTRIBUTE_END } */
    fj_opengl_attribute_t const *attributes;
};

struct fj_opengl_renderer_create_info {
    /** Array of the form { ATTRIBUTE_ID, VALUE, ATTRIBUTE_ID, VALUE, ATTRIBUTE_END } */
    fj_opengl_attribute_t const *attributes;

    /** Specifies the renderer with which the new renderer will share some object IDs. */
    struct fj_opengl_context *friend_renderer;
};


struct fj_opengl_funcs {
    fj_bool8_t (*get_implementation_supported)(fj_opengl_implementation_id_t id);

    fj_opengl_implementation_id_t (*get_default_implementation_id)(void);

    fj_err_t (*create_manager)(
        struct fj_app *owner_app,
        struct fj_opengl_manager **out_manager,
        struct fj_opengl_manager_create_info const *info
    );

    fj_err_t (*destroy_manager)(struct fj_opengl_manager *manager);

    void (*get_manager_internal_info)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_manager_internal_info *out_info
    );

    fj_bool8_t (*get_manager_feature_supported)(
        struct fj_opengl_manager *manager,
        fj_opengl_feature_id_t feature
    );

    /** Creates a standalone off-screen image set. */
    fj_err_t (*create_image_set)(
        struct fj_opengl_manager *manager,
        struct fj_image_set **out_image_set,
        struct fj_size const *image_set_size
    );

    /** Destroys a standalone off-screen image set. */
    fj_err_t (*destroy_image_set)(
        struct fj_opengl_manager *manager,
        struct fj_image_set *image_set
    );

    fj_bool8_t (*get_image_create_capable)(
        struct fj_opengl_manager *manager,
        struct fj_image_set *image_set
    );

    fj_bool8_t (*get_image_create_capable_with_info)(
        struct fj_opengl_manager *manager,
        struct fj_image_set *image_set,
        struct fj_opengl_image_create_info const *image_create_info
    );

    fj_err_t (*create_images)(
        struct fj_opengl_manager *manager,
        struct fj_image_set *image_set,
        struct fj_opengl_image_create_info const *info
    );

    fj_err_t (*destroy_images)(struct fj_opengl_manager *manager, struct fj_image_set *image_set);

    void (*get_image_set_internal_info)(
        struct fj_opengl_manager *manger,
        struct fj_image_set *image_set,
        struct fj_opengl_image_set_internal_info *out_info
    );

    fj_err_t (*swap_images)(struct fj_opengl_manager *manager, struct fj_image_set *image_set);

    fj_err_t (*create_renderer)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_renderer **out_renderer,
        struct fj_image_set *image_set,
        struct fj_opengl_renderer_create_info const *info
    );

    fj_err_t (*destroy_renderer)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_renderer *renderer
    );

    void (*get_renderer_internal_info)(
        struct fj_opengl_renderer *renderer,
        struct fj_opengl_renderer_internal_info *out_info
    );

    fj_opengl_function_getter_t (*get_renderer_function_getter)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_renderer *renderer
    );

    /** Sets the thread-local rendering state. */
    fj_err_t (*begin_rendering)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_renderer *renderer,
        struct fj_image_set *input_image_set,
        struct fj_image_set *output_image_set
    );

    /** Clears the thread-local rendering state. */
    fj_err_t (*end_rendering)(struct fj_opengl_manager *manager);

    /** Useful for saving and restoring the rendering state. */
    fj_err_t (*get_rendering_internal_info)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_rendering_internal_info *out_info
    );

    fj_err_t (*set_rendering_internal_info)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_rendering_internal_info const *info
    );
};


#endif
