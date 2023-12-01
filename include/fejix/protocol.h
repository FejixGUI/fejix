#ifndef FEJIX_PROTOCOL_H_
#define FEJIX_PROTOCOL_H_


#include <fejix/base.h>


#define FJ_PROTOCOL_VERSION_MAKE(MAJOR, MINOR) (((MAJOR)<<16)|(MINOR))
#define FJ_PROTOCOL_VERSION_MAJOR(VERSION) ((VERSION)>>16)
#define FJ_PROTOCOL_VERSION_MINOR(VERSION) ((VERSION)&0x0000FFFF)


typedef uint32_t fj_protocol_version_t;

typedef uint32_t fj_invoke_type_t;

enum fj_invoke_type {
    /** Represents most kinds of main program entrypoints. */
    FJ_INVOKE_TYPE_MAIN,
};

typedef uint32_t fj_interface_id_t;

enum fj_interface_id {
    FJ_IID_PROTOCOL = 1,
    FJ_IID_WINDOW,
    // TODO
};

typedef uint32_t fj_property_id_t;

enum fj_property_id {
    FJ_PID_PROTOCOL_INIT = (FJ_IID_PROTOCOL<<16),
    FJ_PID_WINDOW_INIT = (FJ_IID_WINDOW<<16),
    FJ_PID_WINDOW_SIZE,
    // TODO
};

typedef uint32_t fj_property_flags_t;

enum fj_property_flags {
    FJ_PROPERTY_MONITORABLE = (1<<0),
    FJ_PROPERTY_UPDATABLE = (1<<1),
    FJ_PROPERTY_GETTABLE = (1<<2),

    /** Only regards the GET requests. UPDATE requests are always sync. */
    FJ_PROPERTY_SYNC = (1<<3),
    /** Only regards the GET requests. UPDATE requests are always sync.
        The async requests are generally not obligated to eventually get a
        response. */
    FJ_PROPERTY_ASYNC = (1<<4),
};

typedef uint32_t fj_property_listen_flags_t;

enum fj_property_listen_flags {
    /** Indicates that the client must monitor all the external changes to the
        property if possible.

        If not set, indicates that the client can ignore the external changes.

        === IF UNSUPPORTED ===

        If the property is not monitorable, this flag gets silently ignored. */
    FJ_PROPERTY_LISTEN_MONITOR = (1<<0),
};

typedef uint32_t fj_property_request_flags_t;

enum fj_property_request_flags {
    /** Indicates that the client must update the property with the given value.

        If not set, indicates that the client must get the property value
        and the value argument given to the requestor function must be
        ignored. The value will be received by the property listener callback.

        === IF UNSUPPORTED ===

        If the property is not updatable and this flag is specified, the
        requestor function silently ignores the request.

        If the property is not gettable and this flag is not specified, the
        requestor function silently ignores the request. */
    FJ_PROPERTY_REQUEST_UPDATE = (1<<0),

    /** Indicates that the response of the request must be handled
        synchronously, awaited and processed immediately.

        === IF UNSUPPORTED ===

        If the specified synchronousity is not supported, this flag is ignored.
        */
    FJ_PROPERTY_REQUEST_SYNC = (1<<1),
};

typedef uint32_t fj_property_event_flags_t;

enum fj_property_event_flags {
    /** Indicates that the event being handled is a request for an update.
        That is, the property is being updated.

        If not set, indicates that the property is not being updated. */
    FJ_PROPERTY_EVENT_UPDATE = (1<<0),
};


struct fj_property_event {
    void * state;
    void * FJ_NULLABLE callback_data;
    void * object;
    fj_property_id_t property_id;
    fj_property_event_flags_t event_flags;
    /** Does not have to be the property value. */
    void const * FJ_NULLABLE event_data;

    /** This makes sense only when `FJ_PROPERTY_EVENT_UPDATE` is set.

        If not NULL, indicates that the property update has not been finished
        yet and that the update can be interrupted by the client in order to
        change the value that will be actually assigned to the property.

        If NULL, indicates that the update has already been finished and it
        can be safely assumed that the property already has the specified value.
        */
    void * FJ_NULLABLE interrupt_context;
};

typedef fj_err_t (fj_property_listener_fn_t)(
    struct fj_property_event * event
);

typedef void (fj_property_listener_setter_fn_t)(
    void * object,
    fj_property_listener_fn_t property_listener,
    fj_property_listen_flags_t flags
);


struct fj_method_call_request {
    void * FJ_NULLABLE argument;
};

struct fj_property_request {
    void * object;
    void * FJ_NULLABLE request_data;
    void * FJ_NULLABLE interrupt_context;
    fj_property_request_flags_t flags;
};

union fj_request {
    struct fj_property_request property;
    struct fj_method_call_request method;
};

struct fj_command {
    void const * requestor;
    union fj_request request;
};

struct fj_property {
    fj_property_id_t id;
    fj_property_flags_t flags;
    void const * FJ_NULLABLE requestor;
    fj_property_listener_setter_fn_t * set_listener;
};

struct fj_interface {
    fj_interface_id_t id;

    /** May be 0 if the interface does not have any properties. */
    uint32_t property_count;
    /** This array is sorted by property IDs. */
    struct fj_property const * FJ_NULLABLE FJ_ARRAY properties;

    void const * methods;
};

struct fj_protocol {
    fj_string_t name;

    fj_protocol_version_t version;

    /** Always greater than 0. */
    uint32_t interface_count;
    /** This array is sorted by interface IDs. */
    struct fj_interface const * const * FJ_ARRAY interfaces;

    fj_err_t (* create_state)(
        void * FJ_NULLABLE FJ_OUT * state
    );

    void (* destroy_state)(
        void * state
    );

    void (* set_callback_data)(
        void * state,
        void * FJ_NULLABLE callback_data
    );

    fj_err_t (* invoke)(
        void * state,
        fj_invoke_type_t invoke_type,
        void * FJ_NULLABLE invoke_data
    );

    /** Executes the given commands in the most efficient way.
        The commands may be executed out of order.

        The executed_flags is expected to be the same length as the
        command list and to be initialised to all false. */
    fj_err_t (* execute_commands)(
        void * state,
        uint32_t command_count,
        struct fj_command const * commands,
        fj_bool_t * executed_flags
    );
};


/** The returned array is sorted by protocol names.
    Protocol count is always greater than 0. */
void fj_get_protocols(
    struct fj_protocol const * const * FJ_ARRAY FJ_OUT * protocols,
    uint32_t FJ_OUT * protocol_count
);


#endif
