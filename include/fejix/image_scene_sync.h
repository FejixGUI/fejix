/*
TODO:
throttling modes (wayland frame callback)
    request throttling
    throttle callback
    Note: this may actually be implemented as a special sync method.
sync modes (DwnFlush, CADisplayLink, wayland presentation time, Android choreographer etc.)
scanout modes (vsync, async)
+ request image update
+ on image update callback


Note: this interface is retrieved from the window interface, which must be denoted both in
image_scene_funcs->get_interface_funcs(IMAGE_SCENE_INTERFACE_SYNC)
and
image_scene_sync_funcs->create_manager(owner_image_scene_manager, **out_manager)


*/
