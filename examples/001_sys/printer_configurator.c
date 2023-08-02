#include "printer.h"
#include "printer_configurator.h"


static fj_err_t configure_printing(
    struct fj_sys * sys,
    struct fj_event_data * event_data
)
{
    struct print_event * event = event_data->event;

    event->text = "Hello world! (configured by printer configurator)";

    return FJ_OK;
}


#define CONFIGURATOR_ID 12345

FJ_IMPLEMENT_EVENT_INTERFACE(printer_configurator, configure_printing);

void printer_configurator_init(struct fj_sys * sys)
{
    fj_sys_set_interface(sys, CONFIGURATOR_ID, &printer_configurator);

    fj_sys_bind_event(sys, FJ_GLOBAL_ENTITY, PRINT_EVENT_ID, CONFIGURATOR_ID);
}
