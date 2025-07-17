/** \file */

#ifndef FEJIX_WINDOW_H_
#define FEJIX_WINDOW_H_


#include <fejix/app.h>


struct fj_window_service;
struct fj_window;


/// \addtogroup window_service_dispatching
/// \{

enum fj_window_service_message_type
{
    FJ_WINDOW_SERVICE_INIT,
    FJ_WINDOW_SERVICE_DEINIT,

    FJ_WINDOW_SERVICE_MESSAGE_COUNT,
    FJ_WINDOW_SERVICE_MESSAGE_ENUM_MAX = INT32_MAX,
};

typedef fj_err (*fj_window_service_dispatcher)(
    struct fj_window_service *service,
    enum fj_window_service_message_type type,
    void *message);

/// \}


/// \addtogroup window_service_definition
/// \{

struct fj_window_service_base
{
    fj_window_service_dispatcher dispatch;
    fj_window_service_dispatcher dispatch_default;
    void *user_data;
    struct fj_window_service_data *data;
    struct fj_app *app;
};

struct fj_window_service
#ifndef FJ_OPT_PRIVATE
{
    struct fj_window_service_base base;
}
#endif
;

/// \}


/// \addtogroup window_service_messages
/// \{

struct fj_window_service_init_message
{
    struct fj_window_service **out_service;
    struct fj_window_service_base base;
};

struct fj_window_service_deinit_message
{
    uint8_t _empty;
};

/// \}


/// \addtogroup window_dispatching
/// \{

enum fj_window_message_type
{
    FJ_WINDOW_INIT,
    FJ_WINDOW_DEINIT,
    FJ_WINDOW_UPDATE_TITLE,
    FJ_WINDOW_GET_TITLE,
    FJ_WINDOW_UPDATE_SIZE,
    FJ_WINDOW_GET_SIZE,
    FJ_WINDOW_UPDATE_POSITION,
    FJ_WINDOW_GET_POSITION,
    FJ_WINDOW_UPDATE_VISIBILITY,
    FJ_WINDOW_GET_VISIBILITY,
    FJ_WINDOW_UPDATE_CONTENT,
    FJ_WINDOW_DRAW_CONTENT,

    FJ_WINDOW_MESSAGE_COUNT,
    FJ_WINDOW_MESSAGE_ENUM_MAX = INT32_MAX,
};

typedef fj_err (*fj_window_dispatcher)(
    struct fj_window *window, enum fj_window_message_type type, void *message);

/// \}


/// \addtogroup window_definition
/// \{

struct fj_window_base
{
    fj_window_dispatcher dispatch;
    fj_window_dispatcher dispatch_default;
    void *user_data;
    struct fj_window_data *data;
    struct fj_window_service *service;
};

struct fj_window
#ifndef FJ_OPT_PRIVATE
{
    struct fj_window_base base;
}
#endif
;

/// \}

/// \addtogroup window_messages
/// \{

struct fj_window_init_message
{
    struct fj_window **out_window;
    struct fj_window_base base;
};

struct fj_window_deinit_message
{
    uint8_t _empty;
};

struct fj_window_update_title_message
{
    uint8_t _empty;
};

struct fj_window_get_title_message
{
    char const *out_title;
};

struct fj_window_update_size_message
{
    uint8_t _empty;
};

struct fj_window_get_size_hint_message
{
    struct fj_size2d out_size;
};

/// \}


#endif
