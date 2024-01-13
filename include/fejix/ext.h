#ifndef FEJIX_EXT_H_
#define FEJIX_EXT_H_


#include <fejix/bus.h>


fj_bus_listener_t fj_user_bus_listener;


void fj_ext_get_process_args(
    uint32_t FJ_OUT * argc,
    fj_string_t const * FJ_ARRAY FJ_OUT * argv
);

void fj_ext_set_process_args(
    uint32_t argc,
    fj_string_t const * FJ_ARRAY argv
);

/** Returns the name of the protocol that the program should try to use,
    deduced from the environment.

    First, it tries to read the `FEJIX_PROTOCOL` environment variable.
    If that fails, it tries to read `XDG_SESSION_TYPE`.
    If that fails, returns NULL. */
fj_string_t FJ_NULLABLE fj_ext_get_bus_name_hint(void);

/** Returns the bus name string corresponding to the given bus id.
    The bus name is a NULL-terminated ASCII string of maximum length 32
    (including the NULL terminator). */
fj_string_t fj_ext_get_bus_name(fj_bus_id_t bus_id);

/** If the name is not NULL, searches for the bus with the given name.
    Else, if there is only one bus available, returns that bus.
    Else, returns NULL. */
struct fj_bus const * FJ_NULLABLE fj_ext_get_bus(
    fj_string_t FJ_NULLABLE name_hint
);


/** Runs a common implementation of a graphical client.

    Calls fj_user_bus_listener.

    TODO docs

    === RETURNS ===

    Returns:
    - `0` on success
    - `-1` if fails to find the appropriate bus
    - `-2` if fails to run the program. */
int32_t fj_ext_main(void);


#endif
