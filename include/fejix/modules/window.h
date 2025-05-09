#ifndef FEJIX_WINDOW_H_INCLUDED
#define FEJIX_WINDOW_H_INCLUDED


#include <fejix/modules/drawable.h>


/** \{ */
enum fj_window_event_type {
    FJ_WINDOW_EVENT_DISMISS,
    FJ_WINDOW_EVENT_REDRAW,
    FJ_WINDOW_EVENT_BEGIN_INPUT,
    FJ_WINDOW_EVENT_END_INPUT,
};

struct fj_window_event_dismiss {
    struct fj_window *window;
};

struct fj_window_event_redraw {
    struct fj_window *window;
    struct fj_size2d size;
    fj_density density;
    enum fj_orientation orientation;
};

struct fj_window_event_begin_input {
    struct fj_window *window;
    uint32_t device_group_id;
};

struct fj_window_event_end_input {
    struct fj_window *window;
    uint32_t device_group_id;
};

union fj_window_event_data {
    struct fj_window_event_dismiss *dismiss;
    struct fj_window_event_redraw *redraw;
    struct fj_window_event_begin_input *begin_input;
    struct fj_window_event_end_input *end_input;
};
/** \} */


/** \{ */
enum fj_window_hint_type {
    FJ_WINDOW_HINT_STYLE,
    FJ_WINDOW_HINT_TITLE,
    FJ_WINDOW_HINT_DOCUMENT_PATH,
    FJ_WINDOW_HINT_APPLICATION_ID,
    FJ_WINDOW_HINT_COMPONENT_ID,
    FJ_WINDOW_HINT_DISMISSIBLE,
    FJ_WINDOW_HINT_VISIBLE,
};

enum fj_window_style {
    FJ_WINDOW_STYLE_GENERIC,
    FJ_WINDOW_STYLE_MAIN,
    FJ_WINDOW_STYLE_POPUP_GENERIC,
    FJ_WINDOW_STYLE_POPUP_MENU,
    FJ_WINDOW_STYLE_POPUP_TOOLTIP,
    FJ_WINDOW_STYLE_POPUP_DROPDOWN,
    FJ_WINDOW_STYLE_UTILITY_GENERIC,
};

union fj_window_hint_data {
    enum fj_window_style *style;
    char const *title;
    char const *document_path;
    char const *application_id;
    char const *class_id;
    bool *dismissible;
    bool *visible;
};
/** \} */


FJ_OBJECT_TYPE(fj_window_manager)
FJ_OBJECT_TYPE(fj_window)


FJ_CALLBACK_TYPE(
    fj_window_event_callback,
    enum fj_error,
    struct fj_window_manager *event_object,
    enum fj_window_event_type event_type,
    union fj_window_event_data event_data)


FJ_METHOD_WITH_FALLBACK(
    fj_window_create_manager,
    enum fj_error,
    FJ_ERROR_UNIMPLEMENTED,
    struct fj_window_manager *window_manager,
    struct fj_window_manager **out_manager)

FJ_METHOD(fj_window_destroy_manager, enum fj_error, struct fj_window_manager *manager)

FJ_METHOD(
    fj_window_set_event_callback,
    void,
    struct fj_window_manager *manager,
    fj_window_event_callback callback)

FJ_METHOD(
    fj_window_create,
    enum fj_error,
    struct fj_window_manager *manager,
    struct fj_drawable *drawable,
    struct fj_window **out_window)

FJ_METHOD(
    fj_window_destroy, enum fj_error, struct fj_window_manager *manager, struct fj_window *window)

FJ_METHOD(
    fj_window_sync, enum fj_error, struct fj_window_manager *manager, struct fj_window *window)

FJ_METHOD(
    fj_window_set_hint,
    enum fj_error,
    struct fj_window_manager *manager,
    struct fj_window *window,
    enum fj_window_hint_type hint_type,
    union fj_window_hint_data hint_data)

FJ_METHOD(
    fj_window_request_redraw,
    enum fj_error,
    struct fj_window_manager *manager,
    struct fj_window *window)


FJ_METHOD_LIST_BEGIN(fj_window)
FJ_METHOD_LIST_ITEM(fj_window_create_manager)
FJ_METHOD_LIST_ITEM(fj_window_destroy_manager)
FJ_METHOD_LIST_ITEM(fj_window_set_event_callback)
FJ_METHOD_LIST_ITEM(fj_window_create)
FJ_METHOD_LIST_ITEM(fj_window_destroy)
FJ_METHOD_LIST_ITEM(fj_window_sync)
FJ_METHOD_LIST_ITEM(fj_window_set_hint)
FJ_METHOD_LIST_ITEM(fj_window_request_redraw)
FJ_METHOD_LIST_END()


#endif
