#ifndef FEJIX_LOADER_INCLUDED
#define FEJIX_LOADER_INCLUDED


#include <fejix/core/library.h>


/**
    The returned library object must be unloaded with ``fj_library_unload()``.
*/
FJ_PUBLIC
fj_err_t fj_loader_load_default_library(struct fj_library *out_library);

/**
    Replaces the default no-op dummy function implementations with the functions from the library.
    The missing functions are left no-op.
*/
FJ_PUBLIC
void fj_loader_load_functions(struct fj_library const *library);


#endif
