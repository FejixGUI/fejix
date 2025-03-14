#ifndef FEJIX_UTILS_COLOR_H_INCLUDED
#define FEJIX_UTILS_COLOR_H_INCLUDED


#include <fejix/core.h>


typedef uint32_t fj_color_format_t;

/* Borrowed from the Vulkan specification of VkFormat by Khronos. */
enum {
    FJ_COLOR_FORMAT_R4G4_UNORM_PACK8,
    FJ_COLOR_FORMAT_R4G4B4A4_UNORM_PACK16,
    FJ_COLOR_FORMAT_B4G4R4A4_UNORM_PACK16,
    FJ_COLOR_FORMAT_R5G6B5_UNORM_PACK16,
    FJ_COLOR_FORMAT_B5G6R5_UNORM_PACK16,
    FJ_COLOR_FORMAT_R5G5B5A1_UNORM_PACK16,
    FJ_COLOR_FORMAT_B5G5R5A1_UNORM_PACK16,
    FJ_COLOR_FORMAT_A1R5G5B5_UNORM_PACK16,
    FJ_COLOR_FORMAT_R8_UNORM,
    FJ_COLOR_FORMAT_R8_SNORM,
    FJ_COLOR_FORMAT_R8_UINT,
    FJ_COLOR_FORMAT_R8_SINT,
    FJ_COLOR_FORMAT_R8_SRGB,
    FJ_COLOR_FORMAT_R8G8_UNORM,
    FJ_COLOR_FORMAT_R8G8_SNORM,
    FJ_COLOR_FORMAT_R8G8_UINT,
    FJ_COLOR_FORMAT_R8G8_SINT,
    FJ_COLOR_FORMAT_R8G8_SRGB,
    FJ_COLOR_FORMAT_R8G8B8_UNORM,
    FJ_COLOR_FORMAT_R8G8B8_SNORM,
    FJ_COLOR_FORMAT_R8G8B8_UINT,
    FJ_COLOR_FORMAT_R8G8B8_SINT,
    FJ_COLOR_FORMAT_R8G8B8_SRGB,
    FJ_COLOR_FORMAT_B8G8R8_UNORM,
    FJ_COLOR_FORMAT_B8G8R8_SNORM,
    FJ_COLOR_FORMAT_B8G8R8_UINT,
    FJ_COLOR_FORMAT_B8G8R8_SINT,
    FJ_COLOR_FORMAT_B8G8R8_SRGB,
    FJ_COLOR_FORMAT_R8G8B8A8_UNORM,
    FJ_COLOR_FORMAT_R8G8B8A8_SNORM,
    FJ_COLOR_FORMAT_R8G8B8A8_UINT,
    FJ_COLOR_FORMAT_R8G8B8A8_SINT,
    FJ_COLOR_FORMAT_R8G8B8A8_SRGB,
    FJ_COLOR_FORMAT_B8G8R8A8_UNORM,
    FJ_COLOR_FORMAT_B8G8R8A8_SNORM,
    FJ_COLOR_FORMAT_B8G8R8A8_UINT,
    FJ_COLOR_FORMAT_B8G8R8A8_SINT,
    FJ_COLOR_FORMAT_B8G8R8A8_SRGB,
    FJ_COLOR_FORMAT_A8B8G8R8_UNORM_PACK32,
    FJ_COLOR_FORMAT_A8B8G8R8_SNORM_PACK32,
    FJ_COLOR_FORMAT_A8B8G8R8_USCALED_PACK32,
    FJ_COLOR_FORMAT_A8B8G8R8_SSCALED_PACK32,
    FJ_COLOR_FORMAT_A8B8G8R8_UINT_PACK32,
    FJ_COLOR_FORMAT_A8B8G8R8_SINT_PACK32,
    FJ_COLOR_FORMAT_A8B8G8R8_SRGB_PACK32,
    FJ_COLOR_FORMAT_A2R10G10B10_UNORM_PACK32,
    FJ_COLOR_FORMAT_A2R10G10B10_SNORM_PACK32,
    FJ_COLOR_FORMAT_A2R10G10B10_USCALED_PACK32,
    FJ_COLOR_FORMAT_A2R10G10B10_SSCALED_PACK32,
    FJ_COLOR_FORMAT_A2R10G10B10_UINT_PACK32,
    FJ_COLOR_FORMAT_A2R10G10B10_SINT_PACK32,
    FJ_COLOR_FORMAT_A2B10G10R10_UNORM_PACK32,
    FJ_COLOR_FORMAT_A2B10G10R10_SNORM_PACK32,
    FJ_COLOR_FORMAT_A2B10G10R10_USCALED_PACK32,
    FJ_COLOR_FORMAT_A2B10G10R10_SSCALED_PACK32,
    FJ_COLOR_FORMAT_A2B10G10R10_UINT_PACK32,
    FJ_COLOR_FORMAT_A2B10G10R10_SINT_PACK32,
    FJ_COLOR_FORMAT_R16_UNORM,
    FJ_COLOR_FORMAT_R16_SNORM,
    FJ_COLOR_FORMAT_R16_UINT,
    FJ_COLOR_FORMAT_R16_SINT,
    FJ_COLOR_FORMAT_R16_SFLOAT,
    FJ_COLOR_FORMAT_R16G16_UNORM,
    FJ_COLOR_FORMAT_R16G16_SNORM,
    FJ_COLOR_FORMAT_R16G16_UINT,
    FJ_COLOR_FORMAT_R16G16_SINT,
    FJ_COLOR_FORMAT_R16G16_SFLOAT,
    FJ_COLOR_FORMAT_R16G16B16_UNORM,
    FJ_COLOR_FORMAT_R16G16B16_SNORM,
    FJ_COLOR_FORMAT_R16G16B16_UINT,
    FJ_COLOR_FORMAT_R16G16B16_SINT,
    FJ_COLOR_FORMAT_R16G16B16_SFLOAT,
    FJ_COLOR_FORMAT_R16G16B16A16_UNORM,
    FJ_COLOR_FORMAT_R16G16B16A16_SNORM,
    FJ_COLOR_FORMAT_R16G16B16A16_UINT,
    FJ_COLOR_FORMAT_R16G16B16A16_SINT,
    FJ_COLOR_FORMAT_R16G16B16A16_SFLOAT,
    FJ_COLOR_FORMAT_R32_UINT,
    FJ_COLOR_FORMAT_R32_SINT,
    FJ_COLOR_FORMAT_R32_SFLOAT,
    FJ_COLOR_FORMAT_R32G32_UINT,
    FJ_COLOR_FORMAT_R32G32_SINT,
    FJ_COLOR_FORMAT_R32G32_SFLOAT,
    FJ_COLOR_FORMAT_R32G32B32_UINT,
    FJ_COLOR_FORMAT_R32G32B32_SINT,
    FJ_COLOR_FORMAT_R32G32B32_SFLOAT,
    FJ_COLOR_FORMAT_R32G32B32A32_UINT,
    FJ_COLOR_FORMAT_R32G32B32A32_SINT,
    FJ_COLOR_FORMAT_R32G32B32A32_SFLOAT,
    FJ_COLOR_FORMAT_R64_UINT,
    FJ_COLOR_FORMAT_R64_SINT,
    FJ_COLOR_FORMAT_R64_SFLOAT,
    FJ_COLOR_FORMAT_R64G64_UINT,
    FJ_COLOR_FORMAT_R64G64_SINT,
    FJ_COLOR_FORMAT_R64G64_SFLOAT,
    FJ_COLOR_FORMAT_R64G64B64_UINT,
    FJ_COLOR_FORMAT_R64G64B64_SINT,
    FJ_COLOR_FORMAT_R64G64B64_SFLOAT,
    FJ_COLOR_FORMAT_R64G64B64A64_UINT,
    FJ_COLOR_FORMAT_R64G64B64A64_SINT,
    FJ_COLOR_FORMAT_R64G64B64A64_SFLOAT,
};


#endif
