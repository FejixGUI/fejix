#include "task.h"


static void task_noop(struct fj_task *task)
{
    (void) task;
}


void fj_task_set_completed(struct fj_task *task, fj_err result)
{
    *task = (struct fj_task) {
        .completed = true,
        .result = result,
        .data = 0,
        .poll = task_noop,
        .cancel = task_noop,
    };
}

void fj_task_set_pending(
    struct fj_task *task,
    uintptr_t data,
    void (*poll)(struct fj_task *),
    void (*cancel)(struct fj_task *))
{
    *task = (struct fj_task) {
        .completed = false,
        .result = FJ_ERR_INVALID,
        .data = data,
        .poll = poll,
        .cancel = cancel ? cancel : task_noop,
    };
}
