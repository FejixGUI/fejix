#ifndef FEJIX_EXT_H_
#define FEJIX_EXT_H_


#include <fejix/bus.h>


/** This is the default bus listener which will be used by the library
    on platform implementations that do not allow selecting a custom listener.
    Implement this function and not your own one whenever possible
    so that your code is future-proof and easier to wrap across platforms. */
fj_bus_listener_t fj_ext_user_bus_listener;


void fj_ext_get_process_args(
    uint32_t FJ_OUT * argc,
    fj_string_t const * FJ_NULLABLE FJ_ARRAY FJ_OUT * argv
);

void fj_ext_set_process_args(
    uint32_t argc,
    fj_string_t const * FJ_NULLABLE FJ_ARRAY argv
);

/** Returns the name of the protocol that the program should try to use,
    deduced from the environment.

    First, if there is only one bus available, this function returns its name.
    Otherwise, it tries to read the `FEJIX_BUS` environment variable.
    If that fails, it tries to read `XDG_SESSION_TYPE`.
    If that fails, returns NULL. */
fj_string_t FJ_NULLABLE fj_ext_get_bus_name_hint(void);

/** Returns the bus name string corresponding to the given bus id.
    The bus name is a NULL-terminated ASCII string of maximum length 32
    (including the NULL terminator). */
fj_string_t fj_ext_get_bus_name(fj_id_t bus_id);

/** Searches for the bus with the given name. */
struct fj_bus const * FJ_NULLABLE fj_ext_get_bus(
    fj_string_t name_hint
);


/** Runs a common implementation of a stand-alone graphical client.
    "Stand-alone" means that it runs within a single program entrypoint and
    requires a minimal amount of additional wrappers.

    === RETURNS ===

    Returns:
    - `0` on success
    - `-1` if fails to find the appropriate bus
    - `-2` if fails to serve the bus. */
int32_t fj_ext_standalone_main(fj_bus_listener_t bus_listener);


#endif
