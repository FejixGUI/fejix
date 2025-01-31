#ifndef FEJIX_VULKAN_H_
#define FEJIX_VULKAN_H_


#include <fejix/app.h>
#include <fejix/image_consumer.h>


struct fj_vulkan_global_info {
    /** Pointer to a ``vkGetInstanceProcAddr()`` function pointer. */
    void const *ptr_loader;

    /** Pointer to a ``VkAllocationCallbacks`` structure. */
    void const *ptr_allocator;

    /** Pointer to a ``VkInstance`` handle. */
    void const *ptr_instance;
};


struct fj_vulkan_functions {
    /**
        Calls the appropriate function like ``vkCreateWin32SurfaceKHR()``,
        ``vkCreateWaylandSurfaceKHR()``, ``vkCreateAndroidSurfaceKHR()`` etc.

        The created surface must be destroyed with ``vkDestroySurfaceKHR()``.

        :param out_surface: Pointer to ``VkSurfaceKHR`` to initialise.
        :return: If all the required functions are found, the returned ``VkResult`` is translated
            to a roughly corresponding Fejix error code.
    */
    fj_err_t (*create_surface)(
        struct fj_app *app,
        void *out_surface,
        struct fj_image_consumer *image_consumer,
        struct fj_vulkan_global_info const *global_info);
};


#endif
