#ifndef FEJIX_WINDOW_VIEW_H_INCLUDED
#define FEJIX_WINDOW_VIEW_H_INCLUDED


#include <fejix/modules/window.h>


/** \{ */
enum fj_window_view_event_type {
    FJ_WINDOW_VIEW_EVENT_DISMISS = 0,
    FJ_WINDOW_VIEW_EVENT_CONTENT_UPDATE = 0,
};

struct fj_window_view_event_content_update {
    struct fj_size2d size;
    fj_density density;
    enum fj_orientation orientation;
};

union fj_window_view_event_data {
    struct fj_window_view_event_content_update *content_update;
};
/** \} */


/** \{ */
enum fj_window_view_hint_type {
    FJ_WINDOW_VIEW_HINT_STYLE = 0,
    FJ_WINDOW_VIEW_HINT_TITLE = 1,
    FJ_WINDOW_VIEW_HINT_DOCUMENT_PATH = 2,
    FJ_WINDOW_VIEW_HINT_APPLICATION_ID = 3,
    FJ_WINDOW_VIEW_HINT_COMPONENT_ID = 4,
    FJ_WINDOW_VIEW_HINT_DISMISSIBLE = 5,
    FJ_WINDOW_VIEW_HINT_VISIBLE = 6,
};

enum fj_window_view_style {
    FJ_WINDOW_VIEW_STYLE_GENERIC = 0,
    FJ_WINDOW_VIEW_STYLE_MAIN = 1,
    FJ_WINDOW_VIEW_STYLE_POPUP_GENERIC = 2,
    FJ_WINDOW_VIEW_STYLE_POPUP_MENU = 3,
    FJ_WINDOW_VIEW_STYLE_POPUP_TOOLTIP = 4,
    FJ_WINDOW_VIEW_STYLE_POPUP_DROPDOWN = 5,
    FJ_WINDOW_VIEW_STYLE_UTILITY_GENERIC = 6,
};

union fj_window_view_hint_data {
    enum fj_window_view_style *style;
    char const *title;
    char const *document_path;
    char const *application_id;
    char const *class_id;
    bool *dismissible;
    bool *visible;
};
/** \} */


FJ_OBJECT_TYPE(fj_window_view_manager)
FJ_OBJECT_TYPE(fj_window_view)


FJ_CALLBACK_TYPE(
    fj_window_view_event_callback,
    enum fj_error,
    struct fj_window_view_manager *event_object,
    enum fj_window_view_event_type event_type,
    union fj_window_view_event_data event_data)


FJ_METHOD_WITH_FALLBACK(
    fj_window_view_create_manager,
    enum fj_error,
    FJ_ERROR_UNIMPLEMENTED,
    struct fj_window_manager *window_manager,
    struct fj_window_view_manager **out_manager)

FJ_METHOD(fj_window_view_destroy_manager, enum fj_error, struct fj_window_view_manager *manager)

FJ_METHOD(
    fj_window_view_set_event_callback,
    enum fj_error,
    struct fj_window_view_manager *manager,
    fj_window_view_event_callback callback)

FJ_METHOD(
    fj_window_view_create,
    enum fj_error,
    struct fj_window_view_manager *manager,
    struct fj_window_view **out_view)

FJ_METHOD(
    fj_window_view_destroy,
    enum fj_error,
    struct fj_window_view_manager *manager,
    struct fj_window_view *view)

FJ_METHOD(
    fj_window_view_set_hint,
    enum fj_error,
    struct fj_window_view_manager *manager,
    struct fj_window_view *view,
    enum fj_window_view_hint_type hint_type,
    union fj_window_view_hint_data hint_data)


FJ_METHOD_LIST_BEGIN(fj_window_view)
FJ_METHOD_LIST_ITEM(fj_window_view_create_manager)
FJ_METHOD_LIST_ITEM(fj_window_view_destroy_manager)
FJ_METHOD_LIST_ITEM(fj_window_view_create)
FJ_METHOD_LIST_ITEM(fj_window_view_destroy)
FJ_METHOD_LIST_ITEM(fj_window_view_set_hint)
FJ_METHOD_LIST_END()


#endif
