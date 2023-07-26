#include "printer.h"
#include "printer_configurator.h"


static fj_result_t configure_printing(
    struct fj_sys * sys,
    struct fj_event_data * event_data
)
{
    struct print_event * event = event_data->event;

    event->text = "Hello world! (configured by printer configurator)";

    return FJ_OK;
}


FJ_IMPLEMENT_INTERFACE(fj_event_handler_interface, printer_configurator) {
    FJ_IMPLEMENT_METHOD(handle_event, configure_printing)
};

FJ_DESCRIBE_MODULE(module_description) {
    FJ_DESCRIBE_INTERFACE(PRINT_EVENT_ID, printer_configurator)
    FJ_DESCRIBE_END
};


void printer_configurator_module_init(struct fj_sys * sys)
{
    fj_sys_load_module_description(
        sys, PRINTER_CONFIGURATOR_MODULE_ID, module_description
    );

    fj_sys_bind_event(
        sys, FJ_SYS_GLOBAL_ENTITY, PRINT_EVENT_ID,
        PRINTER_CONFIGURATOR_MODULE_ID
    );
}
