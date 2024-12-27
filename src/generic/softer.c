#include <fejix/interface/softer.h>


size_t fj_softer_get_pixel_size(fj_softer_pixel_format_t format)
{
    if (format <= FJ_SOFTER_PIXEL_FORMAT_SIZE24_MAX)
        return 3;

    if (format <= FJ_SOFTER_PIXEL_FORMAT_SIZE32_MAX)
        return 4;

    return 0;
}
