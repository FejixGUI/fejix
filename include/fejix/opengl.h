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

// TODO: Add emulated OpenGL implementations?
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
    char const *function_name);


struct fj_opengl_manager;
struct fj_opengl_renderer;

struct fj_opengl_thread_state {
    void *_data[6];
};

struct fj_opengl_manager_create_info {
    /** Ignored if NULL. */
    struct fj_opengl_manager_internal_info const *import_internal_info;

    /** Ignored if NULL. */
    fj_opengl_function_getter_t import_function_getter;
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


struct fj_opengl_implementation_funcs {
    fj_opengl_implementation_id_t (*get_implementation_id)(void);

    fj_err_t (*create_manager)(
        struct fj_app *owner_app,
        struct fj_opengl_manager **out_manager,
        struct fj_opengl_manager_create_info const *info);

    fj_err_t (*destroy_manager)(struct fj_opengl_manager *manager);

    /**
        :returns:
            * ``EGLDisplay*`` for EGL.
            * ``Display*`` for GLX.
            * NULL for other implementations.
    */
    void *(*get_manager_internal)(struct fj_opengl_manager *manager);

    /**
        Returns the getter for global platform-specific functions.
        Those are the same functions that are used internally for context creation.

        The returned function getter must only be called with the internal thread state.
    */
    fj_opengl_function_getter_t (*get_manager_function_getter)(struct fj_opengl_manager *manager);

    void (*get_manager_extension_string)(
        struct fj_opengl_manager *manager, char const **out_string);

    fj_bool8_t (*get_manager_feature_supported)(
        struct fj_opengl_manager *manager, fj_opengl_feature_id_t feature);

    /** Creates a standalone off-screen image container. */
    fj_err_t (*create_image_container)(
        struct fj_opengl_manager *manager,
        struct fj_image_container **out_image_container,
        struct fj_size const *image_container_size);

    /** Destroys a standalone off-screen image container. */
    fj_err_t (*destroy_image_container)(
        struct fj_opengl_manager *manager, struct fj_image_container *image_container);

    fj_bool8_t (*get_can_create_images)(
        struct fj_opengl_manager *manager, struct fj_image_container *image_container);

    fj_bool8_t (*get_can_create_images_with_info)(
        struct fj_opengl_manager *manager,
        struct fj_image_container *image_container,
        struct fj_opengl_image_create_info const *image_create_info);

    fj_err_t (*create_images)(
        struct fj_opengl_manager *manager,
        struct fj_image_container *image_container,
        struct fj_opengl_image_create_info const *info);

    fj_err_t (*destroy_images)(
        struct fj_opengl_manager *manager, struct fj_image_container *image_container);

    /**
        :returns:
            * ``EGLSurface*`` for EGL.
            * ``HDC*`` for WGL.
            * ``GLXDrawable*`` for GLX.
    */
    void *(*get_image_internal)(
        struct fj_opengl_manager *manger, struct fj_image_container *image_container);

    fj_err_t (*swap_images)(
        struct fj_opengl_manager *manager, struct fj_image_container *image_container);

    fj_err_t (*create_renderer)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_renderer **out_renderer,
        struct fj_image_container *image_container,
        struct fj_opengl_renderer_create_info const *info);

    fj_err_t (*destroy_renderer)(
        struct fj_opengl_manager *manager, struct fj_opengl_renderer *renderer);

    /**
        :returns:
            * ``EGLContext*`` for EGL.
            * ``HGLRC*`` for WGL.
            * ``GLXContext*`` for GLX.
    */
    void *(*get_renderer_internal)(struct fj_opengl_renderer *renderer);

    /**
        The returned function must only be called under the appropriate thread rendering state.
        That is, the specified rendering context must be current.
    */
    fj_opengl_function_getter_t (*get_renderer_function_getter)(
        struct fj_opengl_manager *manager, struct fj_opengl_renderer *renderer);

    /** Sets the given rendering context as current. */
    fj_err_t (*set_thread_state)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_renderer *renderer,
        struct fj_image_container *input_image_container,
        struct fj_image_container *output_image_container);

    /**
        Sets no rendering context as current.
        Any OpenGL functions that operate on the thread state will fail.
    */
    fj_err_t (*clear_thread_state)(struct fj_opengl_manager *manager);

    fj_err_t (*save_thread_state)(
        struct fj_opengl_manager *manager, struct fj_opengl_thread_state *out_state);

    fj_err_t (*restore_thread_state)(
        struct fj_opengl_manager *manager, struct fj_opengl_thread_state const *state);

    /**
        Sets such a state under which manager functions can be loaded. This typically means that an
        internal manager rendering context and image container are set as current.

        This is used internally in most functions that operate on the manager and create/destroy
        rendering contexts.
    */
    fj_err_t (*set_thread_state_internal)(struct fj_opengl_manager *manager);
};


struct fj_opengl_funcs {
    fj_bool8_t (*get_implementation_supported)(fj_opengl_implementation_id_t id);

    struct fj_opengl_implementation_funcs const *(*get_implementation_funcs)(
        fj_opengl_implementation_id_t id);
};


#endif
