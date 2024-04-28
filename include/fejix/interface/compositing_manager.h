#ifndef FEJIX_COMPOSITING_MANAGER_H_
#define FEJIX_COMPOSITING_MANAGER_H_


#include <fejix/base.h>
#include <fejix/interface/server.h>
#include <fejix/interface/rendering_manager.h>
#include <fejix/interface/display_manager.h>


FJ_DEFINE_HANDLE(fj_compositing_manager_t)
FJ_DEFINE_HANDLE(fj_compositing_context_t)


enum fj_compositing_manager_message_id {
    FJ_COMPOSITING_CONTEXT_SIZE_CHANGED,
};


struct fj_compositing_context_info {
    fj_compositing_context_t *fjOPTION old_context;
    fj_display_context_t *fjOPTION display_context;
    fj_rendering_context_t * rendering_context;
};

struct fj_compositing_context_caps {
    uint32_t max_subcompositing_depth;
};

struct fj_compositing_manager {
    fj_err_t (* create)(
        fj_server_t * server,
        fj_compositing_manager_t *fjOPTION fjOUT * manager
    );

    fj_err_t (* destroy)(
        fj_compositing_manager_t * compositing_manager
    );

    fj_err_t (* create_context)(
        fj_compositing_manager_t * manager,
        struct fj_compositing_context_info const * context_info,
        fj_compositing_context_t *fjOPTION fjOUT * context
    );

    fj_err_t (* destroy_context)(
        fj_compositing_manager_t * manager,
        fj_compositing_context_t * context
    );

    void (* get_context_caps)(
        fj_compositing_manager_t * manager,
        fj_compositing_context_t * context,
        struct fj_compositing_context_caps fjOUT * context_caps
    );
};


#endif
