#ifndef FEJIX_IO_H_INCLUDED
#define FEJIX_IO_H_INCLUDED


#include <fejix/core.h>


#define FJ_PROPERTY_PROTOCOL_ID(PROPERTY_ID) (((PROPERTY_ID) >> 16) & 0xFFFF)
#define FJ_EVENT_PROTOCOL_ID(EVENT_ID) (((EVENT_ID) >> 16) & 0xFFFF)


enum {
    FJ_IO_CONNECTION_IDLE,
    FJ_IO_CONNECTION_FINISH,
    FJ_IO_CONNECTION_PAUSE,
    FJ_IO_CONNECTION_HIBERNATE,
    FJ_IO_CONNECTION_LOW_MEMORY,
};

enum {
    FJ_IO_ELEMENT_USERDATA,
};


typedef uint16_t fj_io_protocol_id_t;

typedef uint32_t fj_io_property_id_t;

typedef uint32_t fj_io_event_id_t;


typedef struct fj_io_connection fj_io_connection_t;

typedef struct fj_io_element fj_io_element_t;


typedef fj_err_t (*fj_io_event_callback_t)(
    fj_io_connection_t *connection,
    fj_io_event_id_t event_id,
    void const *opt_event_data,
    fj_io_element_t *opt_element,
    void *opt_callback_data);


typedef struct fj_io_port {
    char const *name;
    fj_version_t version;

    void const *(*get_protocol)(fj_io_protocol_id_t id);

    fj_err_t (*connect)(
        fj_io_connection_t **out_connection,
        fj_io_event_callback_t event_callback,
        void *opt_callback_data);

    fj_err_t (*disconnect)(fj_io_connection_t *connection);

    fj_err_t (*serve)(fj_io_connection_t *connection);

    fj_err_t (*idle)(fj_io_connection_t *connection);

    fj_err_t (*set_ready_to_finish)(fj_io_connection_t *connection);

    fj_err_t (*synchronize_elements)(
        fj_io_connection_t *connection, fj_io_element_t *const *elements, uint32_t elements_length);

    fj_err_t (*set_element_property)(
        fj_io_element_t *element, fj_io_property_id_t id, void const *property);

    fj_err_t (*get_element_property)(
        fj_io_element_t const *element, fj_io_property_id_t id, void *out_property);
} fj_io_port_t;


FJ_PUBLIC
void fj_io_get_ports(fj_io_port_t const **out_ports, uint32_t *out_ports_length);

FJ_PUBLIC
char const *fj_io_get_default_port_name(void);


#endif
