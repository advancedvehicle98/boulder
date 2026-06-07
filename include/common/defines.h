#ifndef __BOULDER_COMMON_DEFINES_H
#define __BOULDER_COMMON_DEFINES_H


#define __IN
#define __OUT
#define __IO
#define __STATE

#define __INLINE __attribute__(( always_inline )) inline

#ifndef __COLD
#   define __COLD  __attribute__(( cold ))
#endif

#define __HOT      __attribute__(( hot ))
#define __NORETURN __attribute__(( noreturn ))
#define __PACKED   __attribute__(( packed ))


#define _str_equal( L, R ) ( strcmp( L, R ) == 0 )


#define _likely( EXPR )   __builtin_expect( !!( EXPR ), 1 )
#define _unlikely( EXPR ) __builtin_expect( !!( EXPR ), 0 )


#define _assert( EXPR ) typedef int _##__LINE__[ ( EXPR ) ? 1 : -1 ]


#endif // ! __BOULDER_COMMON_DEFINES_H
