#include <src/unixpoller/unixpoller.h>

#include <fejix/core/utils.h>

#include <unistd.h>

#include <math.h>


static fj_err process_events(struct fj_unixpoller *this)
{
    struct pollfd *pollfds = this->pollfds.items;
    fj_unixpoller_callback_fn **callbacks = this->callbacks.items;

    for (size_t i = 0; i < this->pollfds.length; i++) {
        if (pollfds[i].revents == 0) {
            continue;
        }

        FJ_TRY (callbacks[i](this->callback_data, pollfds[i].fd, pollfds[i].revents)) {
            return fj_result;
        }

        pollfds[i].revents = 0;
    }

    return FJ_OK;
}


static fj_err handle_wakeup(
    void *callback_data, fj_unixpoller_fd fd, fj_unixpoller_event_mask events)
{
    (void) callback_data;

    if (events & (POLLERR | POLLHUP | POLLNVAL)) {
        return FJ_ERR_EVENT_WAITING_FAILED;
    }

    uint8_t buffer[1];
    read(fd, buffer, 1);

    return FJ_OK;
}


fj_err fj_unixpoller_init(struct fj_unixpoller *this, void *callback_data)
{
    fj_vec_init(&this->pollfds, sizeof(struct pollfd));
    fj_vec_init(&this->callbacks, sizeof(fj_unixpoller_callback_fn *));

    this->timeout = 0.0;
    this->callback_data = callback_data;

    int pipe_result = pipe((int32_t *) this->wakeup_pipe);

    if (pipe_result < 0) {
        return FJ_ERR_EVENT_WAITING_FAILED;
    }

    FJ_TRY (fj_unixpoller_add(this, this->wakeup_pipe[0], POLLIN, handle_wakeup)) {
        fj_unixpoller_deinit(this);
        return fj_result;
    }

    return FJ_OK;
}


void fj_unixpoller_deinit(struct fj_unixpoller *this)
{
    if (this->wakeup_pipe[0] != 0) {
        close(this->wakeup_pipe[0]);
        close(this->wakeup_pipe[1]);
    }

    fj_vec_deinit(&this->pollfds);
    fj_vec_deinit(&this->callbacks);
}


fj_err fj_unixpoller_add(
    struct fj_unixpoller *this,
    fj_unixpoller_fd file_descriptor,
    fj_unixpoller_event_mask events_to_watch,
    fj_unixpoller_callback_fn *callback)
{
    struct pollfd pollfd = {
        .fd = file_descriptor,
        .events = events_to_watch,
        .revents = 0,
    };

    FJ_TRY (fj_vec_push(&this->pollfds, &pollfd)) {
        return fj_result;
    }

    FJ_TRY (fj_vec_push(&this->callbacks, &callback)) {
        return fj_result;
    }

    return FJ_OK;
}


static fj_err remove_index(struct fj_unixpoller *this, uint32_t index)
{
    FJ_TRY (fj_vec_remove(&this->pollfds, index, 1)) {
        return fj_result;
    }

    FJ_TRY (fj_vec_remove(&this->callbacks, index, 1)) {
        return fj_result;
    }

    return FJ_OK;
}


fj_err fj_unixpoller_remove(struct fj_unixpoller *this, fj_unixpoller_fd file_descriptor)
{
    struct pollfd *pollfds = this->pollfds.items;

    for (size_t i = 0; i < this->pollfds.length; i++) {
        if (pollfds->fd == file_descriptor) {
            FJ_TRY (remove_index(this, i)) {
                return fj_result;
            }

            break;
        }
    }

    return FJ_OK;
}


static int32_t to_poll_timeout(fj_seconds timeout)
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

fj_err fj_unixpoller_poll(struct fj_unixpoller *this)
{
    int32_t result
        = poll(this->pollfds.items, this->pollfds.length, to_poll_timeout(this->timeout));

    if (result < 0) {
        return FJ_ERR_EVENT_WAITING_FAILED;
    }

    if (result == 0) {
        return FJ_OK;
    }

    return process_events(this);
}


void fj_unixpoller_set_timeout(struct fj_unixpoller *this, fj_seconds timeout)
{
    this->timeout = timeout;
}


fj_err fj_unixpoller_wakeup(struct fj_unixpoller *this)
{
    uint8_t buffer[1] = { 42 };
    ssize_t written_count = write(this->wakeup_pipe[1], buffer, 1);

    if (written_count < 0) {
        return FJ_ERR_EVENT_WAITING_FAILED;
    }

    return FJ_OK;
}


fj_bool8 fj_unixpoller_should_finish(struct fj_unixpoller *this)
{
    return isnan(this->timeout);
}
