/**
    \file
*/

#ifndef FEJIX_BACKEND_H_
#define FEJIX_BACKEND_H_


#include <fejix/base.h>


FJ_PUBLIC
void fj_get_backends(char const *const **out_backends, uint32_t *out_backends_length);

/**
    \returns NULL if there are no backends built in, otherwise always returns a valid backend.
*/
FJ_PUBLIC
char const *fj_get_default_backend(void);

FJ_PUBLIC
char const *fj_get_selected_backend(void);

/**
    \param backend_name If NULL, the default backend is initialised.
    \returns Error if the specified backend is not built into the library.
*/
FJ_PUBLIC
enum fj_error fj_select_backend(char const *opt_backend_name);

#endif
