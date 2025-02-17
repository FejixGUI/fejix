#ifndef FEJIX_LIBRARY_INCLUDED
#define FEJIX_LIBRARY_INCLUDED


#include <fejix/core/base.h>


/** Convertible to the actual function type. */
typedef void (*fj_library_function_t)(void);

/**
    Represents a generic entity that just loads functions.
    May be a dynamically loaded library (DLL, dylib) a.k.a. a shared object (SO) file.
*/
struct fj_library {
    void *library_data;
    fj_library_function_t (*load_function)(void *library_data, char const *function_name);
};


/** :param path: UTF-8-encoded library file path. */
FJ_PUBLIC
fj_err_t fj_library_load(struct fj_library *out_library, char const *path);

/**
    This must be called on the ``library`` if only if it was created by ``fj_library_load()``.

    Library structures may be created by the user to emulate function loading from other sources
    than dynamically loaded libraries. This function may thus be not applicable for user-created
    libraries.
*/
FJ_PUBLIC
void fj_library_unload(struct fj_library *library);


#endif
