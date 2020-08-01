#include <assert.h>

#ifdef MEGADEBUG
    #define STRUCT_CHECK(what) \
    if (!what##_OK (This)) \
    { \
        what##_Dump (This); \
        assert (!"OK Failed"); \
    }
#else
    #define STRUCT_CHECK(what)
#endif