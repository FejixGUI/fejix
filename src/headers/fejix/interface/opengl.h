#ifndef FEJIX_OPENGL_INCLUDED
#define FEJIX_OPENGL_INCLUDED


#include <fejix/interface/app.h>
#include <fejix/interface/view.h>


/* Based on khrplatform.h from Khronos. */
#if defined(_WIN32) && !defined(_WIN32_WCE) && !defined(__SCITECH_SNAP__)
#    define FJ_OPENGL_CALL_IMPL __stdcall
#else
#    define FJ_OPENGL_CALL_IMPL
#endif

/** OpenGL ABI calling convention native to the target OS. */
#define FJ_OPENGL_CALL FJ_OPENGL_CALL_IMPL

/** Identifies OpenGL native interface, a.k.a. OpenGL platform. */
typedef uint32_t fj_opengl_platform_id_t;

/** */
enum {
    /** Khronos native platform graphics interface. */
    FJ_OPENGL_PLATFORM_EGL,

    /** Web OpenGL interface. */
    FJ_OPENGL_PLATFORM_WEBGL,

    /** OpenGL over X Window System. */
    FJ_OPENGL_PLATFORM_GLX,

    /** Windows OpenGL interface. */
    FJ_OPENGL_PLATFORM_WGL,

    /** Core OpenGL. */
    FJ_OPENGL_PLATFORM_CGL,
};


typedef uint32_t fj_opengl_feature_flags_t;

enum {
    FJ_OPENGL_FEATURE_GLOBAL_PROTECTED_MODE = 1 << 0,

    /**
        If not present, means that renderer function loaders can only be used for
        renderers of the same image format.

        If present, means that renderer function loaders are essentially the same function and
        can be used for any renderers created for the same manager.
    */
    FJ_OPENGL_FEATURE_RENDERER_SAME_FUNCTIONS = 1 << 1,

    FJ_OPENGL_FEATURE_RENDERER_API_OPENGL_DESKTOP = 1 << 2,
    FJ_OPENGL_FEATURE_RENDERER_API_OPENGL_ES = 1 << 3,
    FJ_OPENGL_FEATURE_RENDERER_FRIEND = 1 << 4,
    FJ_OPENGL_FEATURE_RENDERER_SEPARATE_READ_DRAW = 1 << 5,
    FJ_OPENGL_FEATURE_RENDERER_VSYNC = 1 << 6,
    FJ_OPENGL_FEATURE_RENDERER_ADAPTIVE_VSYNC = 1 << 7,

    /** If not present, means that only the linear colorspace is available. */
    FJ_OPENGL_FEATURE_IMAGE_COLORSPACE_SRGB = 1 << 8,

    FJ_OPENGL_FEATURE_IMAGE_MULTISAMPLING = 1 << 9,
};


typedef uint32_t fj_opengl_attribute_id_t;

enum {
    FJ_OPENGL_ATTRIBUTE_IMAGE_COUNT,
    FJ_OPENGL_ATTRIBUTE_IMAGE_STEREO,
    FJ_OPENGL_ATTRIBUTE_IMAGE_SAMPLES,
    FJ_OPENGL_ATTRIBUTE_IMAGE_RED_BITS,
    FJ_OPENGL_ATTRIBUTE_IMAGE_GREEN_BITS,
    FJ_OPENGL_ATTRIBUTE_IMAGE_BLUE_BITS,
    FJ_OPENGL_ATTRIBUTE_IMAGE_ALPHA_BITS,
    FJ_OPENGL_ATTRIBUTE_IMAGE_ACCUM_RED_BITS,
    FJ_OPENGL_ATTRIBUTE_IMAGE_ACCUM_GREEN_BITS,
    FJ_OPENGL_ATTRIBUTE_IMAGE_ACCUM_BLUE_BITS,
    FJ_OPENGL_ATTRIBUTE_IMAGE_ACCUM_ALPHA_BITS,
    FJ_OPENGL_ATTRIBUTE_IMAGE_DEPTH_BITS,
    FJ_OPENGL_ATTRIBUTE_IMAGE_STENCIL_BITS,
    FJ_OPENGL_ATTRIBUTE_IMAGE_COLORSPACE_SRGB,
    FJ_OPENGL_ATTRIBUTE_IMAGE_PROTECTED,
    // TODO
};

enum {
    FJ_OPENGL_ATTRIBUTE_RENDERER_API_OPENGL_DESKTOP,
    FJ_OPENGL_ATTRIBUTE_RENDERER_API_OPENGL_ES,
    FJ_OPENGL_ATTRIBUTE_RENDERER_API_VERSION_MAJOR,
    FJ_OPENGL_ATTRIBUTE_RENDERER_API_VERSION_MINOR,
    FJ_OPENGL_ATTRIBUTE_RENDERER_API_BACKWARD_COMPATIBLE,
    FJ_OPENGL_ATTRIBUTE_RENDERER_API_FORWARD_COMPATIBLE,
    FJ_OPENGL_ATTRIBUTE_RENDERER_DEBUG_MODE,
    FJ_OPENGL_ATTRIBUTE_RENDERER_PROTECTED,
    FJ_OPENGL_ATTRIBUTE_RENDERER_INDIRECT,
    // TODO
};

typedef int32_t fj_opengl_attribute_value_t;

struct fj_opengl_attribute {
    fj_opengl_attribute_id_t id;
    fj_opengl_attribute_value_t value;
};

struct fj_opengl_attribute_list {
    struct fj_opengl_attribute const *attributes;
    uint32_t attribute_count;
};


typedef void (FJ_OPENGL_CALL *fj_opengl_function_t)(void);

struct fj_opengl_function_loader {
    void *callback_data;

    /**
        The loader for global platform-specific functions.
        Those are the same functions that are used internally for context creation.

        If this returns a non-NULL value, it does not mean that the function is usable.
        Always check for extension avilability before using extension functions.
    */
    fj_opengl_function_t (*load)(void *callback_data, char const *function_name);
};


struct fj_opengl_manager;

/** Resized automatically by the native OpenGL interface. TODO: docs */
struct fj_opengl_images;

struct fj_opengl_renderer;


struct fj_opengl_platform {
    fj_opengl_platform_id_t platform_id;

    struct fj_opengl_function_loader platform_function_loader;

    /**
        Contains:
        * ``EGLDisplay*`` on EGL.
        * ``Display*`` on GLX.
        * NULL on other platforms.
    */
    void *platform_object;
};

/**
    Image formats are equal if at least some fields of this structure are equal.
    Fields that are null must not be considered.

    Note that checking if image formats are equal is not only a rarely used feature on its own,
    but is also somewhat unneeded when ``FJ_OPENGL_FEATURE_GLOBAL_SAME_FUNCTIONS`` is
    available.
*/
struct fj_opengl_image_format {
    uint32_t platform_format_id;
    void *platform_format_object;
};


bool fj_has_opengl(void);

bool fj_opengl_has_platform(fj_opengl_platform_id_t platform_id);

fj_err_t fj_opengl_create_default_platform(
    struct fj_app *owner_app,
    struct fj_opengl_platform **out_platform,
    fj_opengl_platform_id_t platform_id);

fj_err_t fj_opengl_destroy_default_platform(
    struct fj_app *owner_app, struct fj_opengl_platform *platform);

fj_err_t fj_opengl_create_manager(
    struct fj_app *owner_app,
    struct fj_opengl_manager **out_manager,
    struct fj_opengl_platform const *platform);

fj_err_t fj_opengl_destroy_manager(struct fj_opengl_manager *manager);

void fj_opengl_get_manager_platform(
    struct fj_opengl_manager *manager, struct fj_opengl_platform const **out_platform);

/**
    Gets the platform extensions, which are not the same as renderer extensions.

    The renderer extensions must be retrieved with ``glGetString()``.

    :returns: Null-terminated string containing extension names separated by single spaces.
*/
char const *fj_opengl_get_manager_extensions(struct fj_opengl_manager *manager);

fj_opengl_feature_flags_t fj_opengl_get_manager_feature_flags(struct fj_opengl_manager *manager);

fj_err_t fj_opengl_create_offscreen_image_binding(
    struct fj_opengl_manager *manager,
    struct fj_image_binding **out_image_binding,
    struct fj_size const *image_size);

fj_err_t fj_opengl_destroy_offscreen_image_binding(
    struct fj_opengl_manager *manager, struct fj_image_binding *image_binding);

bool fj_opengl_can_bind(struct fj_opengl_manager *manager, struct fj_image_binding *image_binding);

/** Unsupported attributes raise an error. Duplicated attributes are resolved by the last value. */
fj_err_t fj_opengl_create_image_format(
    struct fj_opengl_manager *manager,
    struct fj_opengl_image_format **out_image_format,
    struct fj_opengl_attribute_list const *attribute_list,
    struct fj_image_binding *image_binding);

fj_err_t fj_opengl_destroy_image_format(
    struct fj_opengl_manager *manager, struct fj_opengl_image_format *image_format);

fj_err_t fj_opengl_get_image_format_attribute(
    struct fj_opengl_manager *manager,
    struct fj_opengl_image_format const *image_format,
    fj_opengl_attribute_id_t id,
    fj_opengl_attribute_value_t *out_value);

fj_err_t fj_opengl_create_images(
    struct fj_opengl_manager *manager,
    struct fj_opengl_images **out_images,
    struct fj_opengl_image_format const *image_format,
    struct fj_image_binding *image_binding);

fj_err_t fj_opengl_destroy_images(
    struct fj_opengl_manager *manager, struct fj_opengl_images *images);

void fj_opengl_get_images_image_format(
    struct fj_opengl_manager *manager,
    struct fj_opengl_images *images,
    struct fj_opengl_image_format const **out_image_format);

/**
    :param out_platform_object: Returns:
        * ``EGLSurface*`` on EGL.
        * ``HDC*`` on WGL.
        * ``GLXDrawable*`` on GLX.
        * ``NULL`` on other platforms.
*/
void fj_opengl_get_images_platform_object(
    struct fj_opengl_manager *manager, struct fj_opengl_images *images, void *out_platform_object);

fj_err_t fj_opengl_swap_images(struct fj_opengl_manager *manager, struct fj_opengl_images *images);

/**
    :param friend_renderer: specifies the renderer with which the new renderer will share some
        object IDs.
*/
fj_err_t fj_opengl_create_renderer(
    struct fj_opengl_manager *manager,
    struct fj_opengl_renderer **out_renderer,
    struct fj_opengl_attribute_list attribute_list,
    struct fj_opengl_image_format *image_format,
    struct fj_opengl_renderer *friend_renderer);

fj_err_t fj_opengl_destroy_renderer(
    struct fj_opengl_manager *manager, struct fj_opengl_renderer *renderer);

fj_err_t fj_opengl_get_renderer_attribute(
    struct fj_opengl_manager *manager,
    struct fj_opengl_renderer *renderer,
    fj_opengl_attribute_id_t id,
    fj_opengl_attribute_value_t *out_value);

void fj_opengl_get_renderer_image_format(
    struct fj_opengl_manager *manager,
    struct fj_opengl_renderer *renderer,
    struct fj_opengl_image_format **out_image_format);

/**
    :param out_platform_object: Returns:
        * ``EGLContext*`` on EGL.
        * ``HGLRC*`` on WGL.
        * ``GLXContext*`` on GLX.
        * ``NULL`` on other platforms.
*/
void fj_opengl_get_renderer_platform_object(
    struct fj_opengl_manager *manager,
    struct fj_opengl_renderer *renderer,
    void *out_platform_object);

/**
    The returned function must only be called under the appropriate thread rendering state.
    That is, the specified rendering context must be current.
*/
void fj_opengl_get_renderer_function_getter(
    struct fj_opengl_manager *manager,
    struct fj_opengl_renderer *renderer,
    struct fj_opengl_function_loader const *out_function_loader);

/**
    Sets the given rendering context as current.

    If the ``FJ_OPENGL_FEATURE_RENDERER_SEPARATE_READ_DRAW`` feature is not supported,
    ``read_images`` must be the same as ``draw_images``.
*/
fj_err_t fj_opengl_set_current_in_thread(
    struct fj_opengl_manager *manager,
    struct fj_opengl_renderer *renderer,
    struct fj_opengl_images *read_images,
    struct fj_opengl_images *draw_images);

/**
    Sets no rendering context as current.
    Any OpenGL functions that operate on the thread state will fail.
*/
fj_err_t fj_opengl_unset_current_in_thread(struct fj_opengl_manager *manager);

/**
    Sets such a state under which manager functions can be loaded. This typically means that an
    internal dummy rendering context of the manager is set as current.

    This is used internally in most functions that operate on the manager and create/destroy
    rendering contexts.
*/
fj_err_t fj_opengl_set_dummy_current_in_thread(struct fj_opengl_manager *manager);


#endif
