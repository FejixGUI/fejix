#ifndef FEJIX_LOADER_INCLUDED
#define FEJIX_LOADER_INCLUDED


#include <fejix/core/library.h>


/**
    Loads the functions from a default library (configurable at compile time).
*/
FJ_PUBLIC
fj_err_t fj_loader_load(void);

/**
    Replaces the default no-op dummy function implementations with the functions from the library.
    The missing functions are left no-op.
*/
FJ_PUBLIC
void fj_loader_load_functions(struct fj_library const *library);


#endif
