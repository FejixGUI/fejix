#include <src/unixpoller/unixpoller.h>

#include <fejix/core/malloc.h>
#include <fejix/core/utils.h>

#include <math.h>
#include <unistd.h>


static
fj_err_t process_events(struct fj_unixpoller * this)
{
    FJ_WITH_ERRORS

    struct pollfd * pollfds = this->pollfds.items;
    fj_unixpoller_callback_fn_t * * callbacks = this->callbacks.items;

    for (uint32_t i = 0; i < this->pollfds.length; i++) {

        if (pollfds[i].revents == 0) {
            continue;
        }

        FJ_TRY(callbacks[i](this->callback_data, pollfds[i].fd, pollfds[i].revents)) {
            return FJ_RESULT;
        }

        pollfds[i].revents = 0;
    }

    return FJ_OK;
}


static
fj_err_t handle_wakeup(
    void * callback_data,
    fj_unixpoller_fd_t fd,
    fj_unixpoller_event_mask_t events
)
{
    FJ_ARG_UNUSED(callback_data)

    if (events & (POLLERR|POLLHUP|POLLNVAL)) {
        return FJ_ERR_IO_ERROR;
    }

    uint8_t buffer[1];
    read(fd, buffer, 1);

    return FJ_OK;
}


static
fj_err_t wakeup(struct fj_client_waker const * _this)
{
    FJ_ARG_FROM_OPAQUE(_this, struct fj_unixpoller_waker const * this)

    uint8_t buffer[1] = { 42 };
    ssize_t written_count = write(this->wakeup_fd, buffer, 1);

    if (written_count < 0) {
        return FJ_ERR_IO_ERROR;
    }

    return FJ_OK;
}



fj_err_t fj_unixpoller_init(struct fj_unixpoller * this, void * callback_data)
{
    FJ_WITH_ERRORS

    fj_vec_init(&this->pollfds, sizeof(struct pollfd));
    fj_vec_init(&this->callbacks, sizeof(fj_unixpoller_callback_fn_t *));

    int pipe_result = pipe((int32_t *) this->waker_pipe);

    if (pipe_result < 0) {
        return FJ_ERR_IO_ERROR;
    }

    this->timeout = 0.0;
    this->callback_data = callback_data;
    this->waker.wakeup_fd = this->waker_pipe[1];
    this->waker.base.wakeup = wakeup;

    FJ_TRY(fj_unixpoller_add(this, this->waker.wakeup_fd, POLLIN, handle_wakeup)) {
        fj_unixpoller_deinit(this);
        return FJ_RESULT;
    }

    return FJ_OK;
}


void fj_unixpoller_deinit(struct fj_unixpoller * this)
{
    if (this->waker_pipe[0] != 0) {
        close(this->waker_pipe[0]);
        close(this->waker_pipe[1]);
    }

    fj_vec_deinit(&this->pollfds);
    fj_vec_deinit(&this->callbacks);
}


fj_err_t fj_unixpoller_add(
    struct fj_unixpoller * this,
    fj_unixpoller_fd_t file_descriptor,
    fj_unixpoller_event_mask_t events_to_watch,
    fj_unixpoller_callback_fn_t * callback
)
{
    FJ_WITH_ERRORS

    struct pollfd pollfd = {
        .fd = file_descriptor,
        .events = events_to_watch,
        .revents = 0,
    };

    FJ_TRY(fj_vec_push_item(&this->pollfds, &pollfd)) {
        return FJ_RESULT;
    }

    FJ_TRY(fj_vec_push_item(&this->callbacks, &callback)) {
        return FJ_RESULT;
    }

    return FJ_OK;
}


static
fj_err_t remove_index(struct fj_unixpoller * this, uint32_t index)
{
    FJ_WITH_ERRORS

    FJ_TRY(fj_vec_remove_items(&this->pollfds, index, 1)) {
        return FJ_RESULT;
    }

    FJ_TRY(fj_vec_remove_items(&this->callbacks, index, 1)) {
        return FJ_RESULT;
    }

    return FJ_OK;
}


fj_err_t fj_unixpoller_remove(struct fj_unixpoller * this, fj_unixpoller_fd_t file_descriptor)
{
    FJ_WITH_ERRORS

    struct pollfd * pollfds = this->pollfds.items;

    for (uint32_t i=0; i<this->pollfds.length; i++) {
        if (pollfds->fd == file_descriptor) {
            FJ_TRY(remove_index(this, i)) {
                return FJ_RESULT;
            }

            break;
        }
    }

    return FJ_OK;
}


static
int32_t to_poll_timeout(fj_seconds_t timeout)
{
    if (isinf(timeout)) {
        return -1;
    }

    if (timeout <= 0.0 || isnan(timeout)) {
        return 0;
    }

    // milliseconds
    return (int32_t) (timeout * 1000.0);
}

fj_err_t fj_unixpoller_poll(struct fj_unixpoller * this)
{
    int32_t result = poll(
        this->pollfds.items,
        this->pollfds.length,
        to_poll_timeout(this->timeout)
    );

    if (result < 0) {
        return FJ_ERR_IO_ERROR;
    }

    if (result == 0) {
        return FJ_OK;
    }

    return process_events(this);
}


fj_bool32_t fj_unixpoller_should_finish(struct fj_unixpoller * this)
{
    return isnan(this->timeout);
}
