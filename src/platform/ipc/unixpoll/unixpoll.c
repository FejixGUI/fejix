#include <src/platform/ipc/unixpoll/unixpoll.h>

#include <fejix/malloc.h>
#include <fejix/utils.h>

#include <unistd.h>


static
fj_err_t process_events(
    void * callback_data,
    struct fj_unixpoller * poller
)
{
    FJ_INIT_TRY

    struct pollfd * pollfds = poller->pollfds.items;
    fj_unixpoll_callback_fn_t * * callbacks = poller->callbacks.items;

    for (uint32_t i = 0; i < poller->pollfds.length; i++) {

        if (pollfds[i].revents == 0) {
            continue;
        }

        FJ_TRY
        callbacks[i](callback_data, pollfds[i].fd, pollfds[i].revents);

        FJ_ELSE {
            return FJ_RESULT;
        }

        pollfds[i].revents = 0;
    }

    return FJ_OK;
}


static
fj_err_t process_interruption(
    void * callback_data,
    fj_unixfd_t fd,
    fj_unixpoll_event_mask_t events
)
{
    FJ_UNUSED(callback_data)

    if (events & (POLLERR|POLLHUP|POLLNVAL)) {
        return FJ_ERR_IO_ERROR;
    }

    uint8_t buffer[1];
    read(fd, buffer, 1);

    return FJ_OK;
}


fj_err_t fj_unixpoll_init(struct fj_unixpoller * poller)
{
    FJ_INIT_TRY

    fj_vec_init(&poller->pollfds, sizeof(struct pollfd));
    fj_vec_init(&poller->callbacks, sizeof(fj_unixpoll_callback_fn_t *));

    int pipe_result = pipe((int32_t *) poller->interrupt_pipe);

    if (pipe_result < 0) {
        return FJ_ERR_IO_ERROR;
    }

    FJ_TRY
    fj_unixpoll_add(
        poller,
        fj_unixpoll_get_interruptor(poller),
        POLLIN,
        process_interruption
    );

    FJ_ELSE {
        fj_unixpoll_deinit(poller);
        return FJ_RESULT;
    }

    return FJ_OK;
}


void fj_unixpoll_deinit(struct fj_unixpoller * poller)
{
    if (poller->interrupt_pipe[0] != 0) {
        close(poller->interrupt_pipe[0]);
        close(poller->interrupt_pipe[1]);
    }

    fj_vec_deinit(&poller->pollfds);
    fj_vec_deinit(&poller->callbacks);
}


fj_err_t fj_unixpoll_add(
    struct fj_unixpoller * poller,
    fj_unixfd_t file_descriptor,
    fj_unixpoll_event_mask_t events_to_watch,
    fj_unixpoll_callback_fn_t * callback
)
{
    FJ_INIT_TRY

    struct pollfd pollfd = {
        .fd = file_descriptor,
        .events = events_to_watch,
        .revents = 0,
    };

    FJ_TRY
    fj_vec_push_item(&poller->pollfds, &pollfd);

    FJ_ELSE {
        return FJ_RESULT;
    }

    FJ_TRY
    fj_vec_push_item(&poller->callbacks, &callback);

    FJ_ELSE {
        return FJ_RESULT;
    }

    return FJ_OK;
}


static
fj_err_t fdpoll_remove_index(
    struct fj_unixpoller * poller,
    uint32_t index
)
{
    FJ_INIT_TRY

    FJ_TRY
    fj_vec_remove_items(&poller->pollfds, index, 1);

    FJ_ELSE {
        return FJ_RESULT;
    }

    FJ_TRY
    fj_vec_remove_items(&poller->callbacks, index, 1);

    FJ_ELSE {
        return FJ_RESULT;
    }

    return FJ_OK;
}


fj_err_t fj_unixpoll_remove(
    struct fj_unixpoller * poller,
    fj_unixfd_t file_descriptor
)
{
    FJ_INIT_TRY

    struct pollfd * pollfds = poller->pollfds.items;

    for (uint32_t i=0; i<poller->pollfds.length; i++) {
        if (pollfds->fd == file_descriptor) {
            FJ_TRY
            fdpoll_remove_index(poller, i);

            FJ_ELSE {
                return FJ_RESULT;
            }

            break;
        }
    }

    return FJ_OK;
}


static
int32_t convert_poll_timeout(uint32_t timeout)
{
    if (timeout == UINT32_MAX) {
        return -1;
    }

    if (timeout > UINT16_MAX) {
        return UINT16_MAX;
    }

    return (int32_t) timeout;
}

fj_err_t fj_unixpoll_poll(
    struct fj_unixpoller * poller,
    uint32_t timeout_ms,
    void * callback_data
)
{
    int32_t result = poll(
        poller->pollfds.items,
        poller->pollfds.length,
        convert_poll_timeout(timeout_ms)
    );

    if (result < 0) {
        return FJ_ERR_MESSAGE_RECEIVE_ERROR;
    }

    if (result == 0) {
        return FJ_OK;
    }

    return process_events(callback_data, poller);
}


fj_unixfd_t fj_unixpoll_get_interruptor(
    struct fj_unixpoller * poller
)
{
    return poller->interrupt_pipe[1];
}


fj_err_t fj_unixpoll_interrupt(
    fj_unixfd_t interruptor
)
{
    uint8_t buffer[1] = { 42 };
    ssize_t written_count = write(interruptor, buffer, 1);

    if (written_count < 0) {
        return FJ_ERR_IO_ERROR;
    }

    return FJ_OK;
}
