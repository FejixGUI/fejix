#include <fejix/interface/softer.h>


size_t fj_softer_get_pixel_size(fj_softer_pixel_format_t format)
{
    format &= FJ_SOFTER_PIXEL_FORMAT_COLOR_MASK;

    if (format >= FJ_SOFTER_PIXEL_FORMAT_COLOR_MAX)
        return 0;
    if (format >= FJ_SOFTER_PIXEL_FORMAT_XRGB32)
        return 4;
    return 3;
}
