#ifndef FEJIX_IO_H_INCLUDED
#define FEJIX_IO_H_INCLUDED


#include <fejix/core.h>


struct fj_io_connection;
struct fj_io_element;

struct fj_io_connection_methods {
    fj_err_t (*create)(struct fj_io_connection **out_connection);

    fj_err_t (*destroy)(struct fj_io_connection *connection);
    // TODO
};


#endif
