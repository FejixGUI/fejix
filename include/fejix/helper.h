#ifndef FEJIX_HELPER_H_
#define FEJIX_HELPER_H_


#include <fejix/protocol.h>


typedef fj_err_t (fj_user_loader_fn_t)(
    struct fj_protocol const * protocol,
    void * state
);


fj_user_loader_fn_t fj_user_load;


void fj_helper_get_process_args(
    uint32_t FJ_OUT * argc,
    fj_string_t const * FJ_ARRAY FJ_OUT * argv
);

void fj_helper_set_process_args(
    uint32_t argc,
    fj_string_t const * FJ_ARRAY argv
);

/** Returns the name of the protocol that the program should try to use,
    deduced from the environment.

    First, it tries to read the `FEJIX_PROTOCOL` environment variable.
    If that fails, it tries to read `XDG_SESSION_TYPE`.
    If that fails and there is only a single protocol available, returns its
    name.
    If that fails, returns NULL. */
fj_string_t FJ_NULLABLE fj_helper_get_protocol_hint(void);

/** Runs a common implementation of a graphical client.

    Calls fj_user_load.
    TODO docs

    === RETURNS ===

    Returns 0 on success or -1 on failure. */
int32_t fj_helper_main(void);

/* Returns 0 on success or -1 on failure. */
int fj_helper_cmain(int argc, char const * FJ_ARRAY const * FJ_ARRAY argv);


#endif
