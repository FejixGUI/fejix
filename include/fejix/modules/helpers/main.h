/* clang-format off */
#ifndef FEJIX_HELPER_MAIN_H_INCLUDED
#define FEJIX_HELPER_MAIN_H_INCLUDED

#ifndef FJ_BUILDING_PRIVATE_CODE

#ifdef __cplusplus
extern "C" {
#endif

FJ_HELPER
fj_err fj_main_release(struct fj_main_module *mod)
{
    return mod->methods->release(mod);
}

#ifdef __cplusplus
};
#endif

#endif

#endif
