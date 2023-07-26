#include "printer.h"

#include <stdio.h>


#define MODID PRINTER_MODULE_ID


static void print_impl(struct fj_sys * sys)
{
    struct print_event event;
    event.text = "default_text";
    fj_sys_emit_event(sys, FJ_SYS_GLOBAL_ENTITY, PRINT_EVENT_ID, &event);

    puts(event.text);
}


FJ_IMPLEMENT_INTERFACE(printer, printer_impl) {
    FJ_IMPLEMENT_METHOD(print, print_impl)
};

void printer_module_init(struct fj_sys * sys)
{
    fj_sys_set_interface(sys, MODID, PRINTER_INTERFACE_ID, &printer_impl);
}