#ifndef FEJIX_LOADER_INCLUDED
#define FEJIX_LOADER_INCLUDED


#include <fejix/core/base.h>


/**
    Loads library functions from the shared library file.
    :param library_file_path: The path to the shared library file, encoded in UTF-8. If NULL,
        the default path inferred from the environment is used.
*/
FJ_PUBLIC
fj_err_t fj_loader_load_library(char const *library_file_path);

FJ_PUBLIC
void fj_loader_unload_library(void);


#endif
