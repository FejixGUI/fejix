#ifndef FEJIX_VULKAN_INCLUDED
#define FEJIX_VULKAN_INCLUDED


#include <fejix/interface/app.h>
#include <fejix/interface/image_binding.h>


struct fj_vulkan_global_info {
    /** Pointer to a ``vkGetInstanceProcAddr()`` function pointer. */
    void const *ptr_loader;

    /** Pointer to a ``VkAllocationCallbacks`` structure. */
    void const *ptr_allocator;

    /** Pointer to a ``VkInstance`` handle. */
    void const *ptr_instance;
};


bool fj_has_vulkan(void);

/**
    Calls the appropriate function like ``vkCreateWin32SurfaceKHR()``,
    ``vkCreateWaylandSurfaceKHR()``, ``vkCreateAndroidSurfaceKHR()`` etc.

    The created surface must be destroyed with ``vkDestroySurfaceKHR()``.

    :param out_surface: Pointer to ``VkSurfaceKHR`` to initialise.
    :return: If all the required functions are found, the returned ``VkResult`` is translated
        to a roughly corresponding Fejix error code.
*/
fj_err_t fj_vulkan_create_surface(
    struct fj_app *app,
    void *out_surface,
    struct fj_image_binding *image_binding,
    struct fj_vulkan_global_info const *global_info);


#endif
