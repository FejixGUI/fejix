#ifndef FEJIX_COMMON_TASK_H_
#define FEJIX_COMMON_TASK_H_

#include <fejix/base.h>


void fj_task_set_completed(struct fj_task *task, fj_err result);

/** Initializes the task:
    - fj_task::completed - false
    - fj_task::result - #FJ_ERR_INVALID
    - fj_task::data - \p data
    - fj_task::poll - \p poll
    - fj_task::cancel - \p cancel if not NULL, otherwise a no-op function. */
void fj_task_set_pending(
    struct fj_task *task,
    uintptr_t data,
    void (*poll)(struct fj_task *),
    void (*cancel)(struct fj_task *));


#endif
