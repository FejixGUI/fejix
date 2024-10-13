#include <src/winapi/client/client.h>
#include <src/winapi/scheduler/scheduler.h>

#include <fejix/core/utils.h>


static DWORD get_timeout(struct fj_client *client)
{
    switch (client->scheduler_common.schedule.type) {
        case FJ_SCHEDULE_SLEEP_FOREVER:
            return INFINITE;

        case FJ_SCHEDULE_SLEEP_TIMEOUT:
            return (DWORD) FJ_TIMEOUT_MILLIS(client->scheduler_common.schedule.timeout);
    }

    return 0;
}


fj_bool8_t fj_winapi_scheduler_needs_quit(struct fj_client *client)
{
    return client->scheduler_common.schedule.type == FJ_SCHEDULE_QUIT;
}


void fj_winapi_scheduler_schedule_quit(struct fj_client *client)
{
    client->scheduler_common.schedule.type = FJ_SCHEDULE_QUIT;
    PostQuitMessage(0);
}


fj_err_t fj_winapi_scheduler_schedule_sleep(struct fj_client *client)
{
    if (SendNotifyMessage(client->message_window, FJ_WINAPI_USER_MESSAGE_SLEEP, 0, 0) == FALSE) {
        return FJ_ERR_REQUEST_FAILED;
    }

    return FJ_OK;
}


fj_err_t fj_winapi_scheduler_sleep(struct fj_client *client)
{
    if (client->scheduler_common.callbacks.on_schedule != NULL) {
        FJ_TRY (client->scheduler_common.callbacks.on_schedule(client)) {
            return fj_result;
        }
    }

    if (client->scheduler_common.schedule.type == FJ_SCHEDULE_QUIT) {
        fj_winapi_scheduler_schedule_quit(client);
        return FJ_OK;
    }

    DWORD result = MsgWaitForMultipleObjectsEx(
        0, NULL, get_timeout(client), QS_ALLINPUT, MWMO_INPUTAVAILABLE
    );

    if (result == WAIT_FAILED) {
        return FJ_ERR_EVENT_WAITING_FAILED;
    }

    FJ_TRY (fj_winapi_scheduler_schedule_sleep(client)) {
        return fj_result;
    }

    return FJ_OK;
}


static fj_err_t scheduler_create_common(
    struct fj_client *client,
    struct fj_scheduler_common **common,
    struct fj_scheduler_callbacks const *callbacks,
    struct fj_scheduler_caps *caps
)
{
    *common = &client->scheduler_common;

    (*common)->callbacks = *callbacks;

    caps->timeout_min = FJ_TIMEOUT_FROM_MILLIS(1);
    caps->timeout_max = FJ_TIMEOUT_FROM_MILLIS(INFINITE - 1);
    caps->timeout_max = FJ_TIMEOUT_FROM_MILLIS(1);

    return FJ_OK;
}


static fj_err_t scheduler_destroy_common(
    struct fj_client *client,
    struct fj_scheduler_common *common
)
{
    (void) client;

    common->callbacks = (struct fj_scheduler_callbacks) { NULL };

    return FJ_OK;
}

static void scheduler_set_schedule(
    struct fj_client *client,
    struct fj_scheduler_common *common,
    struct fj_schedule const *schedule
)
{
    (void) client;
    common->schedule.type = schedule->type;
    common->schedule.timeout = schedule->timeout;
}


static fj_err_t scheduler_wakeup(struct fj_client *client, struct fj_scheduler_common *common)
{
    (void) common;

    if (!SendNotifyMessage(client->message_window, FJ_WINAPI_USER_MESSAGE_WAKEUP, 0, 0)) {
        return FJ_ERR_REQUEST_SENDING_FAILED;
    }

    return FJ_OK;
}


struct fj_scheduler_iface const fj_winapi_scheduler_iface = {
    .create_common = scheduler_create_common,
    .destroy_common = scheduler_destroy_common,
    .set_schedule = scheduler_set_schedule,
    .wakeup = scheduler_wakeup,
};
