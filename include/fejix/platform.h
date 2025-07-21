#ifndef FEJIX_PLATFORM_H_
#define FEJIX_PLATFORM_H_


#include <fejix/base.h>


FJ_PUBLIC
void fj_platform_get_builtin_list(
    char const *const **out_platforms, uint32_t *out_platforms_length);

FJ_PUBLIC
char const *fj_platform_load(void);
// TODO Add functions to modify the behavior of the loader, e.g.
// allowed/disallowed platforms etc.

FJ_API_EX(fj_platform_get_name, char const *, NULL, void)

FJ_API_EX(fj_platform_get_version, fj_version, 0, void)


#endif
