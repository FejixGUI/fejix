#include <fejix/ext.h>
#include <fejix/utils.h>

#include <stdlib.h>
#include <stdio.h>


struct program_data {
    fj_bus_listener_t * bus_listener;
    struct fj_bus const * const * FJ_ARRAY buses;
    uint32_t bus_count;
    fj_string_t FJ_NULLABLE bus_name_hint;
    struct fj_bus const * FJ_NULLABLE selected_bus;
};


static
uint32_t global_argc;

static
fj_string_t const * FJ_ARRAY global_argv;

static
fj_string_t bus_names[] = {
    FJ_UTF8("andk"),
    FJ_UTF8("cocoa"),
    FJ_UTF8("uikit"),
    FJ_UTF8("wayland"),
    FJ_UTF8("winapi"),
    FJ_UTF8("x11"),
};


void fj_ext_get_process_args(
    uint32_t FJ_OUT * argc,
    fj_string_t const * FJ_ARRAY FJ_OUT * argv
)
{
    *argc = global_argc;
    *argv = global_argv;
}


void fj_ext_set_process_args(
    uint32_t argc,
    fj_string_t const * FJ_ARRAY argv
)
{
    global_argc = argc;
    global_argv = argv;
}


fj_string_t FJ_NULLABLE fj_ext_get_bus_name_hint(void)
{
    fj_string_t bus = NULL;

    bus = (fj_string_t) getenv("FEJIX_PROTOCOL");

    if (bus != NULL) {
        return bus;
    }

    bus = (fj_string_t) getenv("XDG_SESSION_TYPE");

    if (
        fj_str_eq(bus, FJ_UTF8("x11"))
        || fj_str_eq(bus, FJ_UTF8("wayland"))
    )
    {
        return bus;
    }

    return NULL;
}


fj_string_t fj_ext_get_bus_name(fj_bus_id_t bus_id)
{
    return bus_names[bus_id];
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
void get_buses(struct program_data * data)
{
    fj_get_buses(&data->bus_count, &data->buses);
}


static
fj_err_t select_bus(struct program_data * data)
{
    data->bus_name_hint = fj_ext_get_bus_name_hint();
    data->selected_bus = fj_ext_get_bus(data->bus_name_hint);

    if (data->selected_bus == NULL) {
        return FJ_ERR("cannot select a graphical bus");
    }

    return FJ_OK;
}


static
fj_err_t run_bus(struct program_data * data)
{
    FJ_INIT_ERRORS

    struct fj_bus const * bus = data->selected_bus;
    void * bus_context = NULL;

    FJ_TRY bus->open(&bus_context, data->bus_listener);

    if (FJ_FAILED) {
        return FJ_LAST_ERROR;
    }

    FJ_TRY bus->serve(bus_context, FJ_SERVE_TYPE_MAIN, NULL);

    bus->close(bus_context);

    return FJ_LAST_ERROR;
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
void print_available_buses(struct program_data * data)
{
    fprintf(stderr, "Available buses: ");

    for (uint32_t i = 0; i < data->bus_count; i++) {
        fj_string_t bus_name = fj_ext_get_bus_name(data->buses[i]->id);

        fprintf(stderr, "'%s'", bus_name);

        if (i == data->bus_count - 1) {
            fprintf(stderr, ".\n");
            break;
        }

        fprintf(stderr, ", ");
    }
}


static
void print_error_from_select_bus(struct program_data * data, fj_err_t err)
{
    printf("Error: '%s'\n", err);

    if (data->bus_name_hint != NULL) {
        fprintf(stderr, "Unknown graphical bus '%s'.\n", data->bus_name_hint);
    } else {
        fprintf(stderr, "No graphical bus specified.\n");
    }

    print_available_buses(data);
    print_help();
}


static
void print_error_from_run_bus(struct program_data * data, fj_err_t err)
{
    printf("Error: '%s'\n", err);

    fj_string_t bus_name = fj_ext_get_bus_name(data->selected_bus->id);
    printf("Note: while running graphical bus '%s'\n", bus_name);
}


int32_t fj_ext_standalone_main(fj_bus_listener_t * bus_listener)
{
    FJ_INIT_ERRORS

    struct program_data data = { 0 };

    data.bus_listener = bus_listener;

    get_buses(&data);

    FJ_TRY select_bus(&data);

    if (FJ_FAILED) {
        print_error_from_select_bus(&data, FJ_LAST_ERROR);
        return -1;
    }

    FJ_TRY run_bus(&data);

    if (FJ_FAILED) {
        print_error_from_run_bus(&data, FJ_LAST_ERROR);
        return -2;
    }

    return 0;
}
