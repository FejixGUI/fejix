#include <fejix/ram.h>


size_t fj_ram_get_pixel_size(fj_ram_pixel_format_t format)
{
    if (format <= FJ_RAM_PIXEL_FORMAT_SIZE24_MAX)
        return 3;

    if (format <= FJ_RAM_PIXEL_FORMAT_SIZE32_MAX)
        return 4;

    return 0;
}
