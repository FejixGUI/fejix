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


struct fj_io_connection;

struct fj_io_element;


typedef fj_err_t (*fj_io_event_callback_t)(
    struct fj_io_connection *connection,
    fj_io_event_id_t event_id,
    void const *opt_event_data,
    struct fj_io_element *opt_element,
    void *opt_callback_data);


struct fj_io_port {
    char const *name;
    fj_version_t version;

    void const *(*get_protocol)(fj_io_protocol_id_t id);

    fj_err_t (*connect)(
        struct fj_io_connection **out_connection,
        fj_io_event_callback_t event_callback,
        void *opt_callback_data);

    fj_err_t (*disconnect)(struct fj_io_connection *connection);

    fj_err_t (*serve)(struct fj_io_connection *connection);

    fj_err_t (*idle)(struct fj_io_connection *connection);

    fj_err_t (*set_ready_to_finish)(struct fj_io_connection *connection);

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
