/**
This is the only mandatory interface that an implementation must provide.
*/

#ifndef FEJIX_IMPLEMENTATION_INCLUDED
#define FEJIX_IMPLEMENTATION_INCLUDED


#include <fejix/core/base.h>


/**
    Known return values:
    * "X11"
    * "Wayland"
    * "Android NDK"
    * "Cocoa"
    * "UIKit"
    * "Windows API"
    * "Universal Windows platform"
 */
char const *fj_implementation_get_name(void);

fj_version_t fj_implementation_get_version(void);


#endif
