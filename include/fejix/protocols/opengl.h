#ifndef FJ_OPENGL_H_INCLUDED
#define FJ_OPENGL_H_INCLUDED


#include <fejix/io.h>


enum { FJ_OPENGL_ID = 17 };


typedef struct fj_opengl {
    fj_err_t (*init)(
        fj_io_connection_t *conn,
        opt_native_interface,
        opt_function_loader_callback,
        opt_callback_data,
        opt_native_interface_name);

    fj_err_t (*deinit)(fj_io_connection_t *conn);

    get_native_interface();

    get_native_interface_extensions();

    get_native_interface_features();

    // Dump all creation parameters into create info structs
    create_framebuffer_config(properties, properties_length, native_config, config);

    destroy_framebuffer_config();

    get_framebuffer_config_property();

    fj_err_t (*create_element)(
        fj_io_connection_t *conn, framebuffer_config, fj_io_element_t *out_element);

    fj_err_t (*destroy_element)(fj_io_connection_t *conn, fj_io_element_t *element);

    create_renderer(element, renderer);

    destroy_renderer();

    get_renderer_property();

    present();

    get_thread_function_loader();

    set_thread_state(opt_rend, opt_el_read, opt_el_write);

    get_thread_state();

    clear_thread_state();

    set_native_thread_state();

} fj_opengl_t;


#endif
