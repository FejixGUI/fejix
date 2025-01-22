#ifndef FEJIX_OPENGL_H_
#define FEJIX_OPENGL_H_


#include <fejix/app.h>
#include <fejix/scene.h>


/* Based on khrplatform.h from Khronos. */
#if defined(_WIN32) && !defined(_WIN32_WCE) && !defined(__SCITECH_SNAP__)
#    define FJ_OPENGL_CALL_IMPL __stdcall
#else
#    define FJ_OPENGL_CALL_IMPL
#endif

/** OpenGL ABI calling convention native to the target OS. */
#define FJ_OPENGL_CALL FJ_OPENGL_CALL_IMPL


typedef uint32_t fj_opengl_manager_interface_id_t;

// TODO: Add emulated OpenGL implementations?
enum fj_opengl_manager_interface_id {
    /** OpenGL over X interface. */
    FJ_OPENGL_MANAGER_INTERFACE_GLX,

    /** Khronos native platform graphics interface - (not necessarily) Embedded OpenGL. */
    FJ_OPENGL_MANAGER_INTERFACE_EGL,

    /** Windows OpenGL interface. */
    FJ_OPENGL_MANAGER_INTERFACE_WGL,
};


typedef uint32_t fj_opengl_feature_flags_t;

enum fj_opengl_feature_flags {
    FJ_OPENGL_FEATURE_GLOBAL_SAME_FUNCTION_GETTER = 1 << 0,
    FJ_OPENGL_FEATURE_GLOBAL_PROTECTED_MODE = 1 << 1,
    FJ_OPENGL_FEATURE_MANAGER_SAME_FUNCTION_GETTER = 1 << 2,
    FJ_OPENGL_FEATURE_RENDERER_SAME_FUNCTION_GETTER = 1 << 3,
    FJ_OPENGL_FEATURE_RENDERER_API_OPENGL_DESKTOP = 1 << 4,
    FJ_OPENGL_FEATURE_RENDERER_API_OPENGL_ES = 1 << 5,
    FJ_OPENGL_FEATURE_RENDERER_FRIEND = 1 << 6,
    FJ_OPENGL_FEATURE_RENDERER_SEPARATE_INPUT_OUTPUT = 1 << 7,
    FJ_OPENGL_FEATURE_IMAGE_COLORSPACE_SRGB = 1 << 8,
    FJ_OPENGL_FEATURE_IMAGE_MULTISAMPLING = 1 << 9,
};


typedef uint32_t fj_opengl_attribute_id_t;

enum fj_opengl_image_attribute_id {
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

typedef uint32_t fj_opengl_attribute_value_t;

struct fj_opengl_attribute {
    fj_opengl_attribute_id_t id;
    fj_opengl_attribute_value_t value;
};

struct fj_opengl_attribute_list {
    struct fj_opengl_attribute const *attributes;
    uint32_t attribute_count;
};


typedef void (FJ_OPENGL_CALL *fj_opengl_function_t)(void);

typedef fj_opengl_function_t (FJ_OPENGL_CALL *fj_opengl_function_getter_t)(
    char const *function_name);


struct fj_opengl_manager;

/** Resized automatically by the native OpenGL interface. */
struct fj_opengl_images;

struct fj_opengl_image_format;

struct fj_opengl_renderer;

struct fj_opengl_manager_internal_info {
    /**
        Contains:
        * ``EGLDisplay*`` for EGL.
        * ``Display*`` for GLX.
        * NULL for other implementations.
    */
    void *manager_internal;

    /**
        The getter for global platform-specific functions.
        Those are the same functions that are used internally for context creation.

        This must only be called with the internal thread state.
    */
    fj_opengl_function_getter_t function_getter;
};

struct fj_opengl_manager_create_info {
    /** Ignored if NULL. If any members are NULL, they are ignored. */
    struct fj_opengl_manager_internal_info const *import_internal_info;

    fj_opengl_manager_interface_id_t interface_id;
};

struct fj_opengl_image_create_info {
    struct fj_opengl_attribute_list attributes;
};

struct fj_opengl_renderer_create_info {
    struct fj_opengl_attribute_list attributes;

    /** Specifies the renderer with which the new renderer will share some object IDs. */
    struct fj_opengl_context *friend_renderer;
};


struct fj_opengl_funcs {
    /**
        Always returns at least one interface. The 0-th interface is intended to be the default
        choice if you completely do not care.
    */
    void (*get_supported_manager_interface_ids)(
        fj_opengl_manager_interface_id_t **out_interface_ids, uint32_t *out_count);

    fj_err_t (*create_manager)(
        struct fj_app *owner_app,
        struct fj_opengl_manager **out_manager,
        struct fj_opengl_manager_create_info const *info);

    fj_err_t (*destroy_manager)(struct fj_opengl_manager *manager);

    fj_opengl_manager_interface_id_t (*get_manager_interface_id)(struct fj_opengl_manager *manager);

    fj_opengl_feature_flags_t (*get_manager_feature_flags)(struct fj_opengl_manager *manager);

    void (*get_manager_internal_info)(
        struct fj_opengl_manager *manager, struct fj_opengl_manager_internal_info *out_info);

    /** Creates a standalone off-screen image consumer. */
    fj_err_t (*create_image_consumer)(
        struct fj_opengl_manager *manager,
        struct fj_image_consumer **out_image_consumer,
        struct fj_size const *image_consumer_size);

    /** Destroys a standalone off-screen image consumer. */
    fj_err_t (*destroy_image_consumer)(
        struct fj_opengl_manager *manager, struct fj_image_consumer *image_consumer);

    fj_err_t (*create_image_format)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_image_format **out_image_format,
        struct fj_opengl_attribute_list const *image_attributes,
        struct fj_image_consumer *image_consumer);

    fj_err_t (*destroy_image_format)(
        struct fj_opengl_manager *manager, struct fj_opengl_image_format *image_format);

    fj_err_t (*create_images)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_images **out_images,
        struct fj_opengl_image_format *image_format,
        struct fj_image_consumer *image_consumer);

    fj_err_t (*destroy_images)(struct fj_opengl_manager *manager, struct fj_opengl_images *images);

    /**
        :returns:
            * ``EGLSurface*`` for EGL.
            * ``HDC*`` for WGL.
            * ``GLXDrawable*`` for GLX.
    */
    void *(*get_images_internal)(struct fj_opengl_manager *manger, struct fj_opengl_images *images);

    fj_err_t (*present_image)(struct fj_opengl_manager *manager, struct fj_opengl_images *images);

    fj_err_t (*create_renderer)(
        struct fj_opengl_manager *manager,
        struct fj_opengl_renderer **out_renderer,
        struct fj_opengl_images *images,
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
        struct fj_opengl_images *input_images,
        struct fj_opengl_images *output_images);

    /**
        Sets no rendering context as current.
        Any OpenGL functions that operate on the thread state will fail.
    */
    fj_err_t (*clear_thread_state)(struct fj_opengl_manager *manager);

    /**
        Sets such a state under which manager functions can be loaded. This typically means that an
        internal manager rendering context and images are set as current.

        This is used internally in most functions that operate on the manager and create/destroy
        rendering contexts.
    */
    fj_err_t (*set_thread_state_internal)(struct fj_opengl_manager *manager);
};


#endif
