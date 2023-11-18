#ifndef FEJIX_HELPER_H_
#define FEJIX_HELPER_H_


#include <fejix/protocol.h>


typedef fj_err_t (fj_user_loader_fn_t)(
    struct fj_protocol const * protocol,
    void * state
);


void fj_helper_get_process_args(
    uint32_t FJ_OUT * argc,
    fj_string_t const * FJ_ARRAY FJ_OUT * argv
);

void fj_helper_set_process_args(
    uint32_t argc,
    fj_string_t const * FJ_ARRAY argv
);

/** Runs a common implementation of a graphical client.

    Calls fj_user_load.
    TODO docs

    === RETURNS ===

    Returns 0 on success or -1 on failure. */
int32_t fj_helper_main(void);

fj_user_loader_fn_t fj_user_load;

/* Returns 0 on success or -1 on failure. */
int fj_helper_cmain(
    int argc,
    char const * FJ_ARRAY const * FJ_ARRAY argv
);


#endif
