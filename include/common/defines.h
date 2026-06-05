#ifndef __BOULDER_COMMON_DEFINES_H
#define __BOULDER_COMMON_DEFINES_H


#define __IN
#define __OUT
#define __STATE

#define __INLINE __attribute__(( always_inline )) inline


#define _str_equal( L, R ) ( strcmp( L, R ) == 0 )


#define _likely( EXPR )   __builtin_expect( !!( EXPR ), 1 )
#define _unlikely( EXPR ) __builtin_expect( !!( EXPR ), 0 )


#endif // ! __BOULDER_COMMON_DEFINES_H
