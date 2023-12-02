#ifndef FEJIX_PROTOCOL_H_
#define FEJIX_PROTOCOL_H_


#include <fejix/base.h>


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
    FJ_PID_PROTOCOL_CLIENT_ID,
    FJ_PID_WINDOW_INIT = (FJ_IID_WINDOW<<16),
    FJ_PID_WINDOW_SIZE,
    FJ_PID_WINDOW_TITLE,
    // TODO
};

typedef uint32_t fj_property_flags_t;

enum fj_property_flags {
    FJ_PROPERTY_MONITORABLE = (1<<0),
    FJ_PROPERTY_UPDATABLE = (1<<1),
    FJ_PROPERTY_RETRIEVABLE = (1<<2),

    /** Only regards the RETRIEVE requests. UPDATE requests are always SYNC. */
    FJ_PROPERTY_SYNC = (1<<3),
};

typedef uint32_t fj_property_listen_flags_t;

enum fj_property_listen_flags {
    /** Specifies if the client should monitor the external changes to the
        property.

        If set, indicates that the client must monitor all the external
        changes to the property if possible.
        Note that this is more of a hint and generally there is no guarantee
        that *any* changes will be monitored.

        If not set, indicates that the client can ignore the external changes.

        === IF UNSUPPORTED ===

        If the property is not monitorable, this flag gets silently ignored. */
    FJ_PROPERTY_LISTEN_MONITOR = (1<<0),
};

typedef uint32_t fj_property_request_flags_t;

enum fj_property_request_flags {
    /** Specifies the type of the request.

        If set, indicates that this is an UPDATE request.
        That is, that the client must update the property with the given value.

        If not set, indicates that this is a RETRIEVE request.
        That is, the client must retrieve the property value.
        The request data given to the requestor function is ignored.
        The value will be received by the property listener function.

        === IF UNSUPPORTED ===

        If the specified action is unsupported, the request fails. */
    FJ_PROPERTY_REQUEST_UPDATE = (1<<0),

    /** Specifies the synchronousity of the RETRIEVE request.

        If set, indicates that the request must be SYNC (synchrounous).
        That is, the response must be awaited and processed immediately,
        before the command execution finishes.

        If no set, indicates that the request may be ASYNC (asynchronous).
        That is, the response does not have to be awaited, so it may be
        received some time in the future.

        === NOTES ===

        Generally there is no guarantee that ASYNC requests finish.
        That is, the client may eventually never receive the response of
        an ASYNC request.

        === IF UNSUPPORTED ===

        All UPDATE requests are SYNC anyway, so this flag is ignored for them.

        If the specified synchronousity is unsupported, this flag is ignored
        and the RETRIEVE request proceeds with the supported synchronousity. */
    FJ_PROPERTY_REQUEST_SYNC = (1<<1),
};

typedef uint32_t fj_property_event_flags_t;

enum fj_property_event_flags {
    /** Specifies the type of the request that produced the event.

        If set, indicates that the event was produced by the UPDATE request.
        That is, either the client or the shell have updated the property value.

        If not set, indicates that the event was produced by the RETRIEVE
        request. That is, the client has executed a request to retrieve the
        property value. */
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

        === USAGE ===

        To interrupt a request, the client must execute an UPDATE request
        with the interrupt context inside the property listener during the
        interruptable UDPATE event.
        Such requests are also called UPDATE/INTERRUPT. */
    void * FJ_NULLABLE interrupt_context;
};

typedef fj_err_t (fj_property_listener_fn_t)(struct fj_property_event * event);

typedef void (fj_property_listener_setter_fn_t)(
    void * object,
    fj_property_listener_fn_t property_listener,
    fj_property_listen_flags_t flags
);


struct fj_command {
    void const * requestor;

    union fj_request {
        /** Produces a request to the property.

        === USAGE ===

        For RETRIEVE or UPDATE requests this must be used *outside* the
        property listener, or otherwise the behavior is undefined
        Sometimes, this may result in infinite recursion.

        For UPDATE/INTERRUPT requests this must be used *inside* the property
        listener. */
        struct fj_property_request {
            void * object;
            void * FJ_NULLABLE request_data;
            void * FJ_NULLABLE interrupt_context;
            fj_property_request_flags_t flags;
        } property;

        /** Produces a method call. */
        struct fj_method_call_request {
            void * FJ_NULLABLE argument;
        } method;
    } request;
};

struct fj_property {
    fj_property_id_t id;
    fj_property_flags_t flags;
    void const * requestor;
    fj_property_listener_setter_fn_t * set_listener;
};

struct fj_interface {
    fj_interface_id_t id;

    /** May be 0 if the interface does not have any properties. */
    uint32_t property_count;
    /** This array is sorted by property IDs. */
    struct fj_property const * FJ_NULLABLE FJ_ARRAY properties;

    void const * FJ_NULLABLE methods;
};

struct fj_protocol {
    /** Maximum length of a protocol name is 64 bytes, including the null
        terminator. */
    fj_string_t name;

    fj_version_t version;

    /** Always greater than 0. */
    uint32_t interface_count;
    /** This array is sorted by interface IDs. */
    struct fj_interface const * const * FJ_ARRAY interfaces;

    fj_err_t (* create_state)(void * FJ_NULLABLE FJ_OUT * state);

    void (* destroy_state)(void * state);

    fj_err_t (* invoke)(
        void * state,
        void * FJ_NULLABLE callback_data,
        fj_invoke_type_t invoke_type,
        void * FJ_NULLABLE invoke_data
    );

    /** Executes the given commands in the most efficient way.
        The commands may be executed out of order.

        === ARGS ===

        - `executed_flags` - an array of boolean flags, each one indicates if
            the corresponding command in the `commands` array has been executed.
            This must be the same length as `commands` and must be
            initialised to all false. */
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
