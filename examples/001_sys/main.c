#include "printer.h"
#include "printer_configurator.h"


int main() {
    struct fj_sys * sys = fj_sys_new();

    printer_init(sys);
    printer_configurator_init(sys);

    struct printer * printer = fj_sys_get_interface(sys, PRINTER_INTERFACE_ID);

    printer->print(sys);

    fj_sys_del(sys);

    return 0;
}
