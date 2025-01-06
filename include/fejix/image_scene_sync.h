/*
TODO:
throttling modes (wayland frame callback)
    request throttling
    throttle callback
tuning modes (CADisplayLink, wayland presentation time, Android choreographer etc.)
scanout modes (vsync, async)

window damage?

TODO: this interface is retrieved from the window interface, which must be denoted both in
window_manager->get_interface_funcs(WINDOW_INTERFACE_WINDOW_SYNC)
and
get(window_manager, **out_manager)
*/
