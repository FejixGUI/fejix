#include "printer.h"

#include <stdio.h>


static void print_impl(struct fj_sys * sys)
{
    struct print_event event;
    event.text = "default_text";
    fj_sys_emit_event(sys, FJ_GLOBAL_ENTITY, PRINT_EVENT_ID, &event);

    puts(event.text);
}


FJ_IMPL_BEGIN(printer, printer_impl)
    FJ_IMPL(print, print_impl)
FJ_IMPL_END()

void printer_init(struct fj_sys * sys)
{
    fj_sys_set_interface(sys, PRINTER_INTERFACE_ID, &printer_impl);
}