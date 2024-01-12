#include <fejix/ext.h>
#include <fejix/utils.h>

#include <stdlib.h>
#include <stdio.h>


static
uint32_t _argc;

static
fj_string_t const * FJ_ARRAY _argv;


void fj_ext_get_process_args(
    uint32_t FJ_OUT * argc,
    fj_string_t const * FJ_ARRAY FJ_OUT * argv
)
{
    *argc = _argc;
    *argv = _argv;
}


void fj_ext_set_process_args(
    uint32_t argc,
    fj_string_t const * FJ_ARRAY argv
)
{
    _argc = argc;
    _argv = argv;
}


fj_string_t FJ_NULLABLE fj_ext_get_bus_name_hint(void)
{
    fj_string_t bus = NULL;

    bus = (fj_string_t) getenv("FEJIX_PROTOCOL");

    if (bus != NULL) {
        return bus;
    }

    bus = (fj_string_t) getenv("XDG_SESSION_TYPE");

    if (fj_str_eq(bus, FJ_UTF8("x11"))
     || fj_str_eq(bus, FJ_UTF8("wayland")))
    {
        return bus;
    }

    return NULL;
}


fj_string_t fj_ext_get_bus_name(fj_bus_id_t bus_id)
{
    static
    fj_string_t names[] = {
        FJ_UTF8("andk"), FJ_UTF8("cocoa"), FJ_UTF8("uikit"),
        FJ_UTF8("wayland"), FJ_UTF8("winapi"), FJ_UTF8("x11"),
    };

    return names[bus_id];
}


struct fj_bus const * FJ_NULLABLE fj_ext_get_bus(
    fj_string_t FJ_NULLABLE name_hint
)
{
    uint32_t bus_count;
    struct fj_bus const * const * FJ_ARRAY buses;

    fj_get_buses(&bus_count, &buses);

    if (name_hint == NULL) {
        if (bus_count == 1) {
            return buses[0];
        }

        return NULL;
    }

    for (uint32_t i=0; i<bus_count; i++) {
        fj_string_t bus_name = fj_ext_get_bus_name(buses[i]->id);

        if (fj_str_eq(bus_name, name_hint)) {
            return buses[i];
        }
    }

    return NULL;
}


static
void print_help(void)
{
    fprintf(
        stderr,
        "Note: define 'FEJIX_BUS' or 'XDG_SESSION_TYPE' environment variables"
        " to specify which graphical bus to use.\n"
    );
}


static
void print_available_buses(void)
{
    struct fj_bus const * const * buses;
    uint32_t bus_count;

    fj_get_buses(&bus_count, &buses);

    fprintf(stderr, "Available buses: ");

    for (uint32_t i=0; i<bus_count; i++) {
        fj_string_t bus_name = fj_ext_get_bus_name(buses[i]->id);

        fprintf(stderr, "'%s'", bus_name);

        if (i == bus_count - 1) {
            fprintf(stderr, ".\n");
            break;
        }

        fprintf(stderr, ", ");
    }
}


static
void print_bus_find_error(fj_string_t name_hint)
{
    if (name_hint != NULL) {
        fprintf(stderr, "Unknown graphical bus '%s'.\n", name_hint);
    } else {
        fprintf(stderr, "No graphical bus specified.\n");
    }

    print_available_buses();
    print_help();
}


static
struct fj_bus const * FJ_NULLABLE find_bus_or_print_error(void)
{
    fj_string_t name_hint = fj_ext_get_bus_name_hint();

    struct fj_bus const * bus = fj_ext_get_bus(name_hint);

    if (bus == NULL) {
        print_bus_find_error(name_hint);
        return NULL;
    }

    return bus;
}


static
fj_err_t run_bus(struct fj_bus const * bus)
{
    FJ_INIT_ERRORS

    void * bus_context = NULL;

    FJ_TRY fj_bus_open(bus, &bus_context, fj_user_bus_listener);

    if (FJ_FAILED) {
        return FJ_LAST_ERROR;
    }

    FJ_TRY fj_bus_serve(bus, bus_context, FJ_SERVE_TYPE_MAIN, NULL);

    fj_bus_close(bus, bus_context);

    return FJ_LAST_ERROR;
}


static
fj_err_t run_bus_or_print_error(struct fj_bus const * bus)
{
    FJ_INIT_ERRORS

    FJ_TRY run_bus(bus);

    if (FJ_FAILED) {
        fprintf(stderr, "Error: %s\n", FJ_LAST_ERROR);
        return FJ_LAST_ERROR;
    }

    return FJ_OK;
}


int32_t fj_ext_main(void)
{
    struct fj_bus const * bus = find_bus_or_print_error();

    if (bus == NULL) {
        return -1;
    }

    fj_err_t err = run_bus_or_print_error(bus);

    if (err != FJ_OK) {
        return -1;
    }

    return 0;
}
