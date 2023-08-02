#ifndef PRINTER_H_
#define PRINTER_H_


#include <fejix/fejix.h>


#define PRINTER_INTERFACE_ID 123

FJ_DEFINE_INTERFACE(printer) {
    FJ_DEFINE_METHOD(print, void, (struct fj_sys *))
};


#define PRINT_EVENT_ID 1234

struct print_event {
    fj_string_t text;
};


void printer_init(struct fj_sys * sys);


#endif
