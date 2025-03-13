#ifndef FEJIX_IO_H_INCLUDED
#define FEJIX_IO_H_INCLUDED


#include <fejix/core.h>


typedef uint32_t fj_io_protocol_id_t;

typedef uint32_t fj_io_property_id_t;

typedef uint32_t fj_io_event_id_t;


struct fj_io_connection;

struct fj_io_element;


typedef fj_err_t (*fj_io_event_callback_fn_t)(
    struct fj_io_connection *connection,
    struct fj_io_element *element,
    fj_io_event_id_t event_id,
    void const *event_data);


struct fj_io_port {
    char const *name;
    fj_version_t version;

    void const *(*get_protocol)(fj_io_protocol_id_t id);

    fj_err_t (*create_connection)(
        struct fj_io_connection **out_connection, fj_io_event_callback_fn_t event_callback);

    fj_err_t (*destroy_connection)(struct fj_io_connection *connection);

    fj_err_t (*run_connection)(struct fj_io_connection *connection);

    fj_err_t (*synchronize_elements)(
        struct fj_io_connection *connection,
        struct fj_io_element *const *elements,
        uint32_t elements_length);

    fj_err_t (*set_element_property)(
        struct fj_io_element *element, fj_io_property_id_t id, void const *property);

    fj_err_t (*get_element_property)(
        struct fj_io_element const *element, fj_io_property_id_t id, void *out_property);
};


FJ_PUBLIC
void fj_io_get_ports(struct fj_io_port const **out_ports, uint32_t *out_ports_length);

FJ_PUBLIC
char const *fj_io_get_default_port_name(void);


#endif
