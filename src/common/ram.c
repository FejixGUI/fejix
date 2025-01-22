#include <fejix/ram.h>


size_t fj_ram_get_pixel_size(fj_ram_pixel_format_t format)
{
    if (format >= FJ_RAM_PIXEL_FORMAT_COUNT) {
        return 0;
    }

    if (format >= FJ_RAM_PIXEL_FORMAT_MIN_SIZE32) {
        return 4;
    }

    if (format >= FJ_RAM_PIXEL_FORMAT_MIN_SIZE24) {
        return 3;
    }

    return 0;
}
