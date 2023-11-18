#ifndef FEJIX_PROTOCOL_H_
#define FEJIX_PROTOCOL_H_


#include <fejix/base.h>


typedef uint32_t fj_run_type_t;

enum fj_run_type_values {
    /** Represents most kinds of main program entrypoints. */
    FJ_RUN_TYPE_MAIN,
};

typedef uint32_t fj_class_id_t;

enum fj_class_id_values {
    FJ_CLASS_WINDOW // TODO
};

typedef uint32_t fj_property_id_t;

enum fj_property_id_values {
    FJ_PROPERTY_WINDOW_SIZE // TODO
};

typedef uint32_t fj_property_request_flags_t;

enum fj_property_request_flags_values {
    /** Indicates that the client must update the property with the given value.
        If not set, indicates that the client must request the property value
        and the value argument given to the requestor function must be
        ignored. */
    FJ_PROPERTY_REQUEST_UPDATE = (1<<0),

    /** Indicates that the requestor function does not have to send the request
        immediately. That is, if the protocol encourages caching update
        requests and then sending them all at one batch, the requestor function
        can do exactly that. The cached requests will be sent the next time
        a non-cacheable request is sent.

        If not set, indicates that the requestor function should send the
        request immediately.

        TODO Finish cacheable requests */
    FJ_PROPERTY_REQUEST_CACHEABLE = (1<<1),
};

typedef uint32_t fj_property_event_flags_t;

enum fj_property_event_flags_values {
    /** Indicates that the event being handled is a request for an update.
        That is, the property is being updated.
        If not set, indicates that the property is not being updated. */
    FJ_PROPERTY_EVENT_UPDATING = (1<<0),

    /** Indicates that the property has already been updated.
        If not set, indicates that the event being handled is only a request
        for an update, not the update itself.

        This flag makes sense only when `FJ_PROPERTY_EVENT_UPDATING` is set. */
    FJ_PROPERTY_EVENT_UPDATED = (1<<1),

    /** Indicates that the property is updatable by the client.
        If not set, indicates that update requests sent by the client
        may be rejected. */
    FJ_PROPERTY_EVENT_UPDATABLE = (1<<2),
};

typedef uint32_t fj_class_event_flags_t;

enum fj_class_event_flags_values {
    FJ_CLASS_EVENT_INIT,
    FJ_CLASS_EVENT_DEINIT,
};

typedef fj_err_t (fj_property_requestor_fn_t)(
    void * state,
    void * object,
    fj_property_request_flags_t request_flags,
    void const * FJ_NULLABLE property_value
);

typedef fj_err_t (fj_property_listener_fn_t)(
    void * FJ_NULLABLE callback_data,
    void * object,
    fj_property_id_t property_id,
    fj_property_event_flags_t event_flags,
    void const * FJ_NULLABLE property_value
);

typedef void (fj_property_listener_setter_fn_t)(
    void * object,
    fj_property_listener_fn_t property_listener
);

typedef fj_err_t (fj_class_listener_fn_t)(
    void * callback_data,
    fj_class_id_t class_id,
    fj_class_event_flags_t event_flags
);

typedef void (fj_class_listener_setter_fn_t)(
    void * state,
    fj_class_listener_fn_t * listener
);

struct fj_property {
    fj_property_id_t property_id;
    fj_property_requestor_fn_t * request;
    fj_property_listener_setter_fn_t * set_listener;
};

struct fj_class {
    void const * methods;

    fj_class_listener_setter_fn_t * set_listener;

    /** This array is sorted by property IDs. */
    struct fj_property const * FJ_NULLABLE FJ_ARRAY properties;
    uint32_t property_count;
};

struct fj_protocol {
    void (* get_classes)(
        struct fj_class const * FJ_ARRAY FJ_OUT * classes,
        uint32_t FJ_OUT * count
    );

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

    fj_err_t (* run)(
        void * state,
        fj_run_type_t run_type,
        void * FJ_NULLABLE run_info
    );
};

/** Returns the name of the protocol that the program should try to use.

    First, it tries to read the `FEJIX_PROTOCOL` environment variable.
    If that fails, it tries to read `XDG_SESSION_TYPE`.
    If that fails, it returns the name of the first protocol on the list. */
fj_string_t fj_get_protocol_hint(void);

/** The returned array is sorted by protocol names. */
void fj_get_protocols(
    struct fj_protocol const * FJ_ARRAY FJ_OUT * protocols,
    uint32_t FJ_OUT * protocol_count
);


#endif
