#ifndef __BOULDER_MASTER_DEBUG
#define __BOULDER_MASTER_DEBUG


#ifdef CONFIG_DEBUG
#   define _debug_printf( FMT, ... ) printf( FMT "\n", __VA_ARGS__ )
#   define _debug_dputs( STR ) puts( STR )
#else
#   define _debug_printf( FMT, ... ) ""
#   define _debug_puts( STR ) ""
#endif



#endif // ! __BOULDER_MASTER_DEBUG
