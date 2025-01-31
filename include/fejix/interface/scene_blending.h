/*

on_get_should_blend or something similar - blending is an images creation feature, at least it is so
on X11 and Wayland. It affects OpenGL format choosing behavior. On X11 it affects the window
visual ID, so it is not changeable after the window creation. On Wayland if the user chooses
to create one OpenGL framebuffer for a window, destroy it and then create another, they may wish
to make one transparent and the other one not, which may be possible by calling on_get_should_blend
on every framebuffer creation. That's strange, but the user shouldn't care about callback calling
order anyway.

*/
