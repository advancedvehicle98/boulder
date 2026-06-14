#ifndef __BOULDER_COMMON_DEFINES_H
#define __BOULDER_COMMON_DEFINES_H


#define __IN
#define __OUT
#define __STATE

#ifndef __INLINE
#   define __INLINE __attribute__(( always_inline )) inline
#endif

#ifndef __COLD
#   define __COLD  __attribute__(( cold ))
#endif

#define __HOT      __attribute__(( hot ))
#define __NORETURN __attribute__(( noreturn ))
#define __PACKED   __attribute__(( packed ))
#define __NAKED    __attribute__(( naked ))


#define _str_equal( L, R ) ( strcmp( L, R ) == 0 )


#define _likely( EXPR )   __builtin_expect( !!( EXPR ), 1 )
#define _unlikely( EXPR ) __builtin_expect( !!( EXPR ), 0 )


#define _assert( EXPR ) typedef int _##__LINE__[ ( EXPR ) ? 1 : -1 ]


#define _allowed_can_bitrate_values { 5, 10, 25, 50, 100, 250, 500, 800, 1000 }


#endif // ! __BOULDER_COMMON_DEFINES_H
