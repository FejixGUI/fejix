#ifndef FEJIX_BACKEND_H_INCLUDED
#define FEJIX_BACKEND_H_INCLUDED


#include <fejix/core.h>


FJ_PUBLIC
void fj_backend_get_list(char const *const **out_backends, uint32_t *out_backends_length);

/**
    \returns NULL if there are no backends built in, otherwise always returns a valid backend.
*/
FJ_PUBLIC
char const *fj_backend_get_default(void);

/**
    \returns Error if the specified backend is not built into the library.
*/
FJ_PUBLIC
enum fj_error fj_backend_select(char const *backend_name);


#endif
