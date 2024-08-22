#ifndef FEJIX_BUILTIN_IMPLEMENTATIONS_H_
#define FEJIX_BUILTIN_IMPLEMENTATIONS_H_


#include <fejix/interface/implementation.h>


/** The returned array is sorted by implementation IDs. */
void fj_get_builtin_implementations(
    struct fj_implementation_iface const *const */*[]? out*/ * implementations,
    uint32_t /*out*/ * implementation_count
);


#endif
