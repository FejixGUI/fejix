/** \HEADER */

#ifndef FEJIX_WINDOW_H_
#define FEJIX_WINDOW_H_


#include <fejix/app.h>


struct fj_window_service
{
    fj_dispatcher dispatch;
    fj_dispatcher dispatch_default;
    uintptr_t user_data;
    struct fj_window_service_private_data *data;
    struct fj_app *app;
};


struct fj_window
{
    fj_dispatcher dispatch;
    fj_dispatcher dispatch_default;
    uintptr_t user_data;
    struct fj_window_private_data *data;
    struct fj_window_service *service;
};


enum fj_window_service_message
{
    FJ_WINDOW_SERVICE_INIT,
    FJ_WINDOW_SERVICE_DEINIT,

    FJ_WINDOW_SERVICE_MESSAGE_COUNT,
    FJ_WINDOW_SERVICE_MESSAGE_ENUM_MAX = INT32_MAX,
};


enum fj_window_message
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


#endif
