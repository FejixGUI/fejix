#ifndef PRINTER_H_
#define PRINTER_H_


#include <fejix/fejix.h>


#define PRINTER_INTERFACE_ID 123
#define PRINT_EVENT_ID 1234


struct FJ_INTERFACE(printer) {
    void FJ_METHOD(print)(struct fj_sys *);
};

struct FJ_EVENT(print_event) {
    fj_string_t text;
};


void printer_init(struct fj_sys * sys);


#endif
