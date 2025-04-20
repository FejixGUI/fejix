#include <src/winapi/image_scene/image_scene.h>
#include <src/winapi/utils.h>

#include <fejix/core/utils.h>


enum {
    /** lparam contains *HDWP - a pointer to a DeferWindowPos structure handle.
        If a window needs to change its layout through DeferWindowPos, it should do
        BeginDeferWindowPos (if the handle is NULL) and then DeferWindowPos saving the result into
        the handle behind the pointer. */
    CUSTOM_WINDOW_MESSAGE_UPDATE = WM_USER,
};


static LRESULT CALLBACK window_procedure(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    if (message == WM_CREATE) {
        CREATESTRUCT *window_info = (void *) lparam;
        struct fj_image_scene *image_scene = window_info->lpCreateParams;
        fj_winapi_window_set_data(window, image_scene);
    }

    struct fj_image_scene *image_scene = fj_winapi_window_get_data(window);
    struct fj_image_scene_manager *manager = image_scene->manager;

    // TODO: split this into many functions that handle MSG
    switch (message) {
        case WM_CREATE: {
            image_scene->image_container.type = FJ_WINAPI_IMAGE_CONTAINER_WINDOW;
            image_scene->image_container.window = image_scene->window;

            // TODO: image scene startup size via image_scene_layout

            // TODO: image scene visibility control with image_scene_layout
            // TODO: show windows on image creation
            // ShowWindow(window, SW_SHOWNORMAL);

            return 0;
        }

        case WM_DESTROY: {
            FJ_FREE(image_scene);

            return 0;
        }

        case WM_DPICHANGED: {
            // TODO: handle window resizing during DPI changes with image_scene_layout
            // TODO: Is SetWindowPos optimal on DPI changes?
            // TODO: utils to get rect x/y/width/height
            WORD dpi = LOWORD(wparam);
            RECT *suggested_rect = (void *) lparam;
            int x = suggested_rect->left;
            int y = suggested_rect->top;
            int width = suggested_rect->right - suggested_rect->left;
            int height = suggested_rect->bottom - suggested_rect->top;
            SetWindowPos(window, NULL, x, y, width, height, SWP_NOOWNERZORDER);

            RECT rect;
            GetClientRect(window, &rect);

            double density = (double) dpi / FJ_INCH_LENGTH;
            int client_width = rect.right - rect.left;
            int client_height = rect.top - rect.bottom;

            struct fj_image_scene_geometry_info info = {
                .density = density,
                .orientation = FJ_ORIENTATION_NORMAL,
                .size = { .width = client_width, .height = client_height },
            };
            manager->callbacks->on_geometry_update(manager->app, image_scene, &info);

            return 0;
        }

        // TODO: Handle WM_WINDOWPOSCHANING with image_scene_layout
        case WM_WINDOWPOSCHANGED: {
            WINDOWPOS *windowpos = (void *) lparam;
            double density = (double) GetDpiForWindow(window) / FJ_INCH_LENGTH;

            struct fj_image_scene_geometry_info info = {
                .orientation = FJ_ORIENTATION_NORMAL,
                .size = { .width = windowpos->cx, .height = windowpos->cy },
                .density = density,
            };

            return 0;
        }

        case WM_CLOSE: {
            // TODO: image scene dismissing prevention with image_scene_layout
            CloseWindow(window);

            manager->callbacks->on_dismissed(manager->app, image_scene);

            return 0;
        }

        default:
            return DefWindowProc(window, message, wparam, lparam);
    }
}


static void const *get_interface_functions(fj_image_scene_interface_id id)
{
    return NULL;
}


static enum fj_error create_manager(
    struct fj_app *owner_app,
    struct fj_image_scene_manager **out_manager,
    struct fj_image_scene_manager_create_info const *info)
{
    FJ_TRY (FJ_ALLOC_ZEROED(out_manager)) {
        return fj_result;
    }

    struct fj_image_scene_manager *manager = *out_manager;
    *manager = (struct fj_image_scene_manager) {
        .app = owner_app,
        .callbacks = info->callbacks,
    };

    return FJ_OK;
}


static enum fj_error destroy_manager(struct fj_image_scene_manager *manager)
{
    FJ_FREE(manager);
    return FJ_OK;
}


static enum fj_error create_image_scene(
    struct fj_image_scene_manager *manager,
    struct fj_image_scene **out_image_scene,
    struct fj_image_scene_create_info const *info)
{
    FJ_TRY (FJ_ALLOC_ZEROED(out_image_scene)) {
        return fj_result;
    }

    struct fj_image_scene *image_scene = *out_image_scene;
    image_scene->tag = info->tag;

    WNDCLASSEX class_info = {
        .lpfnWndProc = window_procedure,
    };

    CREATESTRUCT window_info = {
        .style = WS_OVERLAPPEDWINDOW,
        .x = CW_USEDEFAULT,
        .y = CW_USEDEFAULT,
        .cx = CW_USEDEFAULT,
        .cy = CW_USEDEFAULT,
        .lpCreateParams = image_scene,
    };

    FJ_TRY (fj_winapi_window_create(&image_scene->window, &class_info, &window_info)) {
        FJ_FREE(image_scene);
        return fj_result;
    }

    return FJ_OK;
}


static enum fj_error destroy_image_scene(
    struct fj_image_scene_manager *manager, struct fj_image_scene *image_scene)
{
    (void) manager;

    if (DestroyWindow(image_scene->window) == FALSE) {
        return FJ_ERROR_REQUEST_FAILED;
    }

    FJ_FREE(image_scene);
    return FJ_OK;
}


static void get_image_container(
    struct fj_image_scene_manager *manager,
    struct fj_image_scene *image_scene,
    struct fj_image_container **out_image_container)
{
    (void) manager;
    *out_image_container = &image_scene->image_container;
}


static enum fj_error image_scene_update(
    struct fj_image_scene_manager *manager,
    struct fj_image_scene *const *image_scenes,
    uint32_t image_scene_count)
{
    HDWP defer_window_pos_state = NULL;

    for (uint32_t i = 0; i < image_scene_count; i++) {
        SendMessage(
            image_scenes[i]->window,
            CUSTOM_WINDOW_MESSAGE_UPDATE,
            NULL,
            (LONG_PTR) &defer_window_pos_state);
    }

    if (defer_window_pos_state != NULL) {
        if (EndDeferWindowPos(defer_window_pos_state) == 0) {
            return FJ_ERROR_REQUEST_FAILED;
        }
    }

    return FJ_OK;
}


struct fj_image_scene_functions const fj_winapi_image_scene_functions = {
    .get_interface_functions = get_interface_functions,
    .create_manager = create_manager,
    .destroy_manager = destroy_manager,
    .create_image_scene = create_image_scene,
    .destroy_image_scene = destroy_image_scene,
    .get_image_container = get_image_container,
    .update = image_scene_update,
};
