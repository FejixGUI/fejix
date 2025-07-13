#include "x11.h"

#include <X11/Xlib-xcb.h>

#include <src/shared/common/error.h>
#include <src/shared/common/macros.h>

#include <string.h>

// From <Xproto.h>
// Maps the request opcodes to their names
static char const *const x11_request_names[] = {
    [1] = "CreateWindow",
    [2] = "ChangeWindowAttributes",
    [3] = "GetWindowAttributes",
    [4] = "DestroyWindow",
    [5] = "DestroySubwindows",
    [6] = "ChangeSaveSet",
    [7] = "ReparentWindow",
    [8] = "MapWindow",
    [9] = "MapSubwindows",
    [10] = "UnmapWindow",
    [11] = "UnmapSubwindows",
    [12] = "ConfigureWindow",
    [13] = "CirculateWindow",
    [14] = "GetGeometry",
    [15] = "QueryTree",
    [16] = "InternAtom",
    [17] = "GetAtomName",
    [18] = "ChangeProperty",
    [19] = "DeleteProperty",
    [20] = "GetProperty",
    [21] = "ListProperties",
    [22] = "SetSelectionOwner",
    [23] = "GetSelectionOwner",
    [24] = "ConvertSelection",
    [25] = "SendEvent",
    [26] = "GrabPointer",
    [27] = "UngrabPointer",
    [28] = "GrabButton",
    [29] = "UngrabButton",
    [30] = "ChangeActivePointerGrab",
    [31] = "GrabKeyboard",
    [32] = "UngrabKeyboard",
    [33] = "GrabKey",
    [34] = "UngrabKey",
    [35] = "AllowEvents",
    [36] = "GrabServer",
    [37] = "UngrabServer",
    [38] = "QueryPointer",
    [39] = "GetMotionEvents",
    [40] = "TranslateCoords",
    [41] = "WarpPointer",
    [42] = "SetInputFocus",
    [43] = "GetInputFocus",
    [44] = "QueryKeymap",
    [45] = "OpenFont",
    [46] = "CloseFont",
    [47] = "QueryFont",
    [48] = "QueryTextExtents",
    [49] = "ListFonts",
    [50] = "ListFontsWithInfo",
    [51] = "SetFontPath",
    [52] = "GetFontPath",
    [53] = "CreatePixmap",
    [54] = "FreePixmap",
    [55] = "CreateGC",
    [56] = "ChangeGC",
    [57] = "CopyGC",
    [58] = "SetDashes",
    [59] = "SetClipRectangles",
    [60] = "FreeGC",
    [61] = "ClearArea",
    [62] = "CopyArea",
    [63] = "CopyPlane",
    [64] = "PolyPoint",
    [65] = "PolyLine",
    [66] = "PolySegment",
    [67] = "PolyRectangle",
    [68] = "PolyArc",
    [69] = "FillPoly",
    [70] = "PolyFillRectangle",
    [71] = "PolyFillArc",
    [72] = "PutImage",
    [73] = "GetImage",
    [74] = "PolyText8",
    [75] = "PolyText16",
    [76] = "ImageText8",
    [77] = "ImageText16",
    [78] = "CreateColormap",
    [79] = "FreeColormap",
    [80] = "CopyColormapAndFree",
    [81] = "InstallColormap",
    [82] = "UninstallColormap",
    [83] = "ListInstalledColormaps",
    [84] = "AllocColor",
    [85] = "AllocNamedColor",
    [86] = "AllocColorCells",
    [87] = "AllocColorPlanes",
    [88] = "FreeColors",
    [89] = "StoreColors",
    [90] = "StoreNamedColor",
    [91] = "QueryColors",
    [92] = "LookupColor",
    [93] = "CreateCursor",
    [94] = "CreateGlyphCursor",
    [95] = "FreeCursor",
    [96] = "RecolorCursor",
    [97] = "QueryBestSize",
    [98] = "QueryExtension",
    [99] = "ListExtensions",
    [100] = "ChangeKeyboardMapping",
    [101] = "GetKeyboardMapping",
    [102] = "ChangeKeyboardControl",
    [103] = "GetKeyboardControl",
    [104] = "Bell",
    [105] = "ChangePointerControl",
    [106] = "GetPointerControl",
    [107] = "SetScreenSaver",
    [108] = "GetScreenSaver",
    [109] = "ChangeHosts",
    [110] = "ListHosts",
    [111] = "SetAccessControl",
    [112] = "SetCloseDownMode",
    [113] = "KillClient",
    [114] = "RotateProperties",
    [115] = "ForceScreenSaver",
    [116] = "SetPointerMapping",
    [117] = "GetPointerMapping",
    [118] = "SetModifierMapping",
    [119] = "GetModifierMapping",
};

static char const *const x11_error_messages[] = {
    [Success] = "Success: everything is okay",
    [BadRequest] = "BadRequest: bad request code",
    [BadValue] = "BadValue: int parameter out of range",
    [BadWindow] = "BadWindow: parameter not a Window",
    [BadPixmap] = "BadPixmap: parameter not a Pixmap",
    [BadAtom] = "BadAtom: parameter not an Atom",
    [BadCursor] = "BadCursor: parameter not a Cursor",
    [BadFont] = "BadFont: parameter not a Font",
    [BadMatch] = "BadMatch: parameter mismatch",
    [BadDrawable] = "BadDrawable: parameter not a Pixmap or Window",
    [BadAccess] = "BadAccess: (depends on context)",
    [BadAlloc] = "BadAlloc: insufficient resources",
    [BadColor] = "BadColor: no such colormap",
    [BadGC] = "BadGC: parameter not a GC",
    [BadIDChoice] = "BadIDChoice: choice not in range or already used",
    [BadName] = "BadName: font or color name doesn't exist",
    [BadLength] = "BadLength: Request length incorrect",
    [BadImplementation] = "BadImplementation: server is defective",
};

char const *fj_x11_get_error_message(uint8_t error_code)
{
    if (error_code >= FJ_LEN(x11_error_messages)
        || x11_error_messages[error_code] == NULL)
    {
        return "<non-standard error>";
    }

    return x11_error_messages[error_code];
}

static char const *get_error_request_name(xcb_generic_error_t *error)
{
    if (error->major_code >= FJ_LEN(x11_request_names)
        || x11_request_names[error->major_code] == NULL)
    {
        return "<request name not implemented yet>";
    }

    return x11_request_names[error->major_code];
}

char const *fj_x11_xcb_error_into_string(xcb_generic_error_t *error)
{
    return fj_x11_get_error_message(error->error_code);
}


fj_err fj_x11_check(xcb_connection_t *c, xcb_void_cookie_t coockie)
{
    xcb_generic_error_t *error = xcb_request_check(c, coockie);
    if (error) {
        FJ_ERRORF(
            "request \"%s\" failed: %s",
            get_error_request_name(error),
            fj_x11_xcb_error_into_string(error));
        free(error);
        return FJ_ERR_REJECTED;
    }
    return FJ_OK;
}


static int xlib_error_handler(Display *display, XErrorEvent *error)
{
    (void) display;
    FJ_ERRORF("Xlib error: %s", fj_x11_get_error_message(error->error_code));
    return 0;
}

fj_err fj_x11_connect(Display **out_display, xcb_connection_t **out_connection)
{
    *out_display = XOpenDisplay(NULL);
    if (*out_display == NULL) {
        FJ_ERROR("cannot open X11 display");
        return FJ_ERR_SYSTEM;
    }

    XSetErrorHandler(xlib_error_handler);

    *out_connection = XGetXCBConnection(*out_display);
    XSetEventQueueOwner(*out_display, XCBOwnsEventQueue);

    return FJ_OK;
}

fj_err fj_x11_disconnect(Display *display)
{
    int result = XCloseDisplay(display);
    if (result != 0) {
        FJ_ERRORF(
            "cannot close X11 display: %s", fj_x11_get_error_message(result));
    }

    return FJ_OK;
}


fj_err fj_x11_flush(xcb_connection_t *c)
{
    if (xcb_flush(c) <= 0)
        return FJ_ERR_SYSTEM;

    return FJ_OK;
}


fj_err fj_x11_get_atoms(
    xcb_connection_t *c, xcb_atom_t atoms[FJ_X11_ATOM_COUNT])
{
    fj_err e;

#undef FJ_MACROLIST
#undef FJ_MACROITEM
#define FJ_MACROITEM(ITEM) [FJ_X11_ATOM(ITEM)] = #ITEM,
#define FJ_MACROLIST(...) \
    static char const *const atom_names[FJ_X11_ATOM_COUNT] = { __VA_ARGS__ };

    FJ_X11_ATOM_MACROLIST

    xcb_intern_atom_cookie_t coockies[FJ_X11_ATOM_COUNT];

    for (enum fj_x11_atom atom = 0; atom < FJ_X11_ATOM_COUNT; atom++) {
        coockies[atom]
            = xcb_intern_atom(c, 1, strlen(atom_names[atom]), atom_names[atom]);
    }

    e = xcb_flush(c);
    if (e)
        return e;

    for (enum fj_x11_atom atom = 0; atom < FJ_X11_ATOM_COUNT; atom++) {
        xcb_intern_atom_reply_t *reply
            = xcb_intern_atom_reply(c, coockies[atom], NULL);

        atoms[atom] = reply->atom;

        free(reply);
    }

    return FJ_OK;
}


xcb_void_cookie_t fj_x11_create_window(
    xcb_connection_t *c,
    xcb_screen_t *screen,
    struct fj_x11_create_window_data *data)
{
    if (screen == NULL)
        screen = xcb_setup_roots_iterator(xcb_get_setup(c)).data;

    if (data->parent == 0)
        data->parent = screen->root;

    uint32_t properties[] = {
        screen->black_pixel, data->override_redirect,
        data->event_mask,    data->colormap,
        data->cursor,
    };

    uint32_t property_mask = XCB_CW_BORDER_PIXEL | XCB_CW_OVERRIDE_REDIRECT
        | XCB_CW_EVENT_MASK | XCB_CW_COLORMAP | XCB_CW_CURSOR;

    return xcb_create_window(
        c,
        data->depth,
        data->window,
        data->parent,
        (int16_t) data->viewport.offset.x,
        (int16_t) data->viewport.offset.y,
        (uint16_t) data->viewport.size.width,
        (uint16_t) data->viewport.size.height,
        0,  // border width
        data->window_class,
        data->visual,
        property_mask,
        properties);
}
