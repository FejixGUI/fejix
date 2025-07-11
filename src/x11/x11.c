#include "x11.h"

#include <src/shared/common/macros.h>


char const *fj_x11_error_into_string(uint8_t error_code)
{
    static char const *const messages[] = {
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

    if (error_code >= FJ_LEN(messages)) {
        return "non-standard error";
    }

    return messages[error_code];
}

char const *fj_x11_xcb_error_into_string(xcb_generic_error_t *error)
{
    return fj_x11_error_into_string(error->error_code);
}
