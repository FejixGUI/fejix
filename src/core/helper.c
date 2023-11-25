#include <fejix/helper.h>
#include <fejix/utils.h>

#include <stdlib.h>
#include <stdio.h>


static uint32_t _argc;
static fj_string_t const * FJ_ARRAY _argv;


void fj_helper_get_process_args(
    uint32_t FJ_OUT * argc,
    fj_string_t const * FJ_ARRAY FJ_OUT * argv
)
{
    *argc = _argc;
    *argv = _argv;
}


void fj_helper_set_process_args(
    uint32_t argc,
    fj_string_t const * FJ_ARRAY argv
)
{
    _argc = argc;
    _argv = argv;
}


fj_string_t FJ_NULLABLE fj_helper_get_protocol_hint(void)
{
    fj_string_t protocol = NULL;

    protocol = (fj_string_t) getenv("FEJIX_PROTOCOL");

    if (protocol != NULL) {
        return protocol;
    }

    protocol = (fj_string_t) getenv("XDG_SESSION_TYPE");

    if (fj_str_eq(protocol, FJ_UTF8("x11"))
     || fj_str_eq(protocol, FJ_UTF8("wayland")))
    {
        return protocol;
    }

    uint32_t protocol_count;
    struct fj_protocol const * const * protocols;
    fj_get_protocols(&protocols, &protocol_count);

    if (protocol_count == 1) {
        return protocols[0]->name;
    }

    return NULL;
}


static struct fj_protocol const * FJ_NULLABLE find_protocol(
    fj_string_t protocol_hint,
    struct fj_protocol const * const * protocols,
    uint32_t protocol_count
)
{
    for (uint32_t i=0; i<protocol_count; i++) {
        if (fj_str_eq(protocol_hint, protocols[i]->name)) {
            return protocols[i];
        }
    }

    return NULL;
}


static void print_protocols(
    struct fj_protocol const * const * protocols,
    uint32_t protocol_count
)
{
    fprintf(stderr, "Build-in protocols: ");

    for (uint32_t i=0; i<protocol_count; i++) {
        fprintf(stderr, "'%s'", protocols[i]->name);

        if (i == protocol_count-1) {
            fprintf(stderr, ".\n");
        } else {
            fprintf(stderr, ", ");
        }
    }
}


/** Returns NULL on failure. Prints error to stderr. */
static struct fj_protocol const * FJ_NULLABLE get_protocol(void)
{
    struct fj_protocol const * const * protocols;
    uint32_t protocol_count;
    fj_get_protocols(&protocols, &protocol_count);

    fj_string_t protocol_hint = fj_helper_get_protocol_hint();

    if (protocol_hint == NULL) {
        fprintf(stderr, "Cannot guess protocol.\n");
        print_protocols(protocols, protocol_count);
        return NULL;
    }

    struct fj_protocol const * protocol = find_protocol(
        protocol_hint, protocols, protocol_count
    );

    if (protocol == NULL) {
        fprintf(stderr, "Unknown protocol '%s'.", protocol_hint);
        print_protocols(protocols, protocol_count);
        return NULL;
    }

    return protocol;
}


fj_err_t protocol_main_run(struct fj_protocol const * protocol, void * state)
{
    FJ_INIT_ERRORS

    FJ_TRY fj_user_load(protocol, state);

    if (FJ_FAILED) {
        return FJ_LAST_ERROR;
    }

    FJ_TRY protocol->run(state, FJ_RUN_TYPE_MAIN, NULL);

    return FJ_LAST_ERROR;
}


fj_err_t protocol_main(struct fj_protocol const * protocol)
{
    FJ_INIT_ERRORS

    void * state = NULL;

    FJ_TRY protocol->create_state(&state);

    if (FJ_FAILED) {
        return FJ_LAST_ERROR;
    }

    FJ_TRY protocol_main_run(protocol, state);

    if (FJ_FAILED) {
        protocol->destroy_state(state);
        return FJ_LAST_ERROR;
    }

    protocol->destroy_state(state);

    return FJ_OK;
}


int32_t fj_helper_main(void)
{
    struct fj_protocol const * protocol = get_protocol();

    if (protocol == NULL) {
        return -1;
    }

    fj_err_t err = protocol_main(protocol);

    if (err != FJ_OK) {
        fprintf(stderr, "Error: %s\n", err);
        return -1;
    }

    return 0;
}


int fj_helper_cmain(int argc, char const * FJ_ARRAY const * FJ_ARRAY argv)
{
    fj_helper_set_process_args((uint32_t) argc, (void *) argv);
    return fj_helper_main();
}
