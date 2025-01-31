#ifndef FEJIX_LOADER_H_
#define FEJIX_LOADER_H_


#include <fejix/core/base.h>


/** :returns: The pointer to the requested function or NULL. */
typedef void const *(*fj_loader_load_function_fn_t)(void *callback_data, char const *function_name);

struct fj_loader {
    fj_loader_load_function_fn_t load_function;
    void *userdata;
};

fj_err_t fj_loader_load(struct fj_loader const *loader);

fj_err_t fj_loader_open_library(char const *library_path, struct fj_loader *out_loader);

fj_err_t fj_loader_close_library(struct fj_loader const *loader);

char const *fj_loader_get_default_library_path(void);

#endif
