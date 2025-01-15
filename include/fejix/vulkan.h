#ifndef FEJIX_VULKAN_H_
#define FEJIX_VULKAN_H_


#include <fejix/app.h>
#include <fejix/image_container.h>

#ifndef FJ_OPT_NO_VULKAN_HEADERS
#    include <vulkan/vulkan.h>
#endif


struct fj_vulkan_manager_create_info {
    VkInstance instance;
    VkAllocationCallbacks const *allocation_callbacks;
    PFN_vkGetInstanceProcAddr function_getter;
};


struct fj_vulkan_manager;


struct fj_vulkan_funcs {
    fj_err_t (*create_manager)(
        struct fj_app *owner_app,
        struct fj_vulkan_manager **out_manager,
        struct fj_vulkan_manager_create_info const *info);

    fj_err_t (*destroy_manager)(struct fj_vulkan_manager *manager);

    fj_bool8_t (*get_image_create_capable)(
        struct fj_vulkan_manager *manager, struct fj_image_container *image_container);

    fj_err_t (*get_image_container_surface)(
        struct fj_vulkan_manager *manager,
        struct fj_image_container *image_container,
        VkSurface *out_surface);

    fj_err_t (*release_image_container_surface)(
        struct fj_vulkan_manager *manager,
        struct fj_image_container *image_container,
        VkSurface surface);
};


#endif
