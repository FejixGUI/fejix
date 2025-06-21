/**
    \file
*/

#ifndef FEJIX_RUTIME_H_
#define FEJIX_RUTIME_H_


#include <fejix/base.h>


FJ_PUBLIC
void (*fj_log_callback)(
    enum fj_log_level log_level,
    char const *file,
    uint32_t line,
    char const *function,
    char const *message);

/**
    \returns NULL on allocation failure or when freeing.
*/
FJ_PUBLIC
void *(*fj_alloc_callback)(void *pointer, size_t old_size, size_t new_size);

FJ_PUBLIC
void fj_get_backends(char const *const **out_backends, uint32_t *out_backends_length);

/**
    \returns NULL if there are no backends built in, otherwise always returns a valid backend.
*/
FJ_PUBLIC
char const *fj_get_default_backend(void);

/**
    \param backend_name If NULL, the default backend is initialised.
    \returns Error if the specified backend is not built into the library.
*/
FJ_PUBLIC
enum fj_error fj_init(char const *backend_name);

#endif
