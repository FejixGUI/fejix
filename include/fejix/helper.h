#ifndef FEJIX_HELPER_H_
#define FEJIX_HELPER_H_


#include <fejix/base.h>
#include <fejix/gprotocol.h>


void fj_helper_get_process_args(
    uint32_t * FJ_OUT argc,
    fj_string_t const * FJ_ARRAY * FJ_OUT argv
);

void fj_helper_set_process_args(
    uint32_t argc,
    fj_string_t const * FJ_ARRAY argv
);

/** Runs a common implementation of a graphical client.

    TODO docs

    === RETURNS ===

    Returns 0 on success or -1 on failure. */
int32_t fj_helper_main(void);

/* Returns 0 on success or -1 on failure. */
int fj_helper_cmain(
    int argc,
    char const * const FJ_ARRAY * FJ_ARRAY argv
);


#endif
