#ifndef FEJIX_INTERNAL_LOADER_INCLUDED
#define FEJIX_INTERNAL_LOADER_INCLUDED

#include <stdint.h>

typedef void (*fj_loader_function_t)(void);

extern char const *fj_loader_function_names[];
extern fj_loader_function_t *fj_loader_function_pointers[];
extern uint32_t fj_loader_function_count;

#endif
