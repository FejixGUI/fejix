#define FJ_INTERNAL_OPT_DEFINE_DEFAULT_API_IMPLEMENTATIONS

// Suppress warnings about unused parameters in the default implementations

// This applies to both GCC and Clang
#if defined(__GNUC__)
#    pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
// This applies to Visual C
#if defined(_MSC_VER)
#    pragma warning(disable : 4100)
#endif

#include <fejix/app.h>
#include <fejix/timer.h>
#include <fejix/window.h>
