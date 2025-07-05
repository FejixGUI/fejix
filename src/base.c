#include <fejix/base.h>

static void poll_(struct fj_task *task)
{
    (void) task;
}

static void cancel_(struct fj_task *task)
{
    (void) task;
}

void fj_task_init_completed(struct fj_task *out_task)
{
    *out_task = (struct fj_task) {
        .poll = poll_,
        .cancel = cancel_,
        .internal_data = 0,
        .result = FJ_OK,
        .completed = true,
    };
}
