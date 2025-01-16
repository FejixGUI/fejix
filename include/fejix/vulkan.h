#ifndef FEJIX_VULKAN_H_
#define FEJIX_VULKAN_H_


#include <fejix/app.h>
#include <fejix/image_container.h>


struct fj_vulkan_global_info {
    /** Pointer to a ``vkGetInstanceProcAddr()`` function pointer. */
    void const *ptr_loader;

    /** Pointer to a ``VkAllocationCallbacks`` structure. */
    void const *ptr_allocator;

    /** Pointer to a ``VkInstance`` handle. */
    void const *ptr_instance;
};


struct fj_vulkan_funcs {
    fj_bool8_t (*get_can_create_images)(
        struct fj_app *app,
        struct fj_image_container *image_container,
        struct fj_vulkan_global_info const *global_info);

    /**
        Calls the appropriate function like ``vkCreateWin32SurfaceKHR()``,
        ``vkCreateXcbSurfaceKHR()`` etc.

        The created surface must be destroyed with ``vkDestroySurfaceKHR()``.

        :param out_surface: Pointer to ``VkSurfaceKHR`` to initialise.
        :return: If all the required functions are found, the returned ``VkResult`` is translated
            to a roughly corresponding Fejix error code.
    */
    fj_err_t (*create_surface)(
        struct fj_app *app,
        struct fj_image_container *image_container,
        struct fj_vulkan_global_info const *global_info,
        void *out_surface);
};


#endif
