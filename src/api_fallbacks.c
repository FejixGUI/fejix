// This applies to both GCC and Clang
#if defined(__GNUC__)
#    pragma GCC diagnostic ignored "-Wunused-parameter"
// Microsoft Visual C compiler
#elif defined(MSVC)
#    pragma warning(disable : 4100)
#endif


#define FJ_OPT_DEFINE_API_FALLBACKS

#include <fejix/app.h>
#include <fejix/window.h>
