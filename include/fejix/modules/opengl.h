#ifndef FJ_OPENGL_H_INCLUDED
#define FJ_OPENGL_H_INCLUDED


#include <fejix/io.h>


enum { FJ_OPENGL_ID = 17 };


typedef struct fj_opengl {
    fj_err (*init)(
        fj_io_connection *conn,
        opt_native_interface,
        opt_function_loader_callback,
        opt_callback_data,
        opt_native_interface_name);

    fj_err (*deinit)(fj_io_connection *conn);

    get_native_interface();

    get_native_interface_extensions();

    get_native_interface_features();

    // Dump all creation parameters into create info structs
    create_framebuffer_config(properties, properties_length, native_config, config);

    destroy_framebuffer_config();

    get_framebuffer_config_property();

    fj_err (*create_element)(
        fj_io_connection *conn, framebuffer_config, fj_io_element *out_element);

    fj_err (*destroy_element)(fj_io_connection *conn, fj_io_element *element);

    create_renderer(element, renderer);

    destroy_renderer();

    get_renderer_property();

    present();

    get_thread_function_loader();

    set_thread_state(opt_rend, opt_el_read, opt_el_write);

    get_thread_state();

    clear_thread_state();

    set_native_thread_state();

} fj_opengl;


#endif
