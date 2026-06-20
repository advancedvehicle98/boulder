#include <slave/math.h>

#include <common/config.h>
#include <common/defines.h>


// см. http://web.archive.org/web/20180517023231/http://www.hackersdelight.org/divcMore.pdf


__NAKED void
slave_arch_math_divmod10( __OUT       uint32_t *div,
						  __OUT       uint32_t *mod,
						  __IN  const uint32_t  x )
{
#ifdef CONFIG_SLAVE_FAST_MATH
	
	static uint8_t table[ 16 ] = {
		0, 1, 2, 2, 3, 4, 5,
		5, 6, 7, 7, 8, 9, 0
	};
	
	__asm__ volatile (
	// divu10 -------------------------------------------
	  
	// q = ( n >> 1 ) + ( n >> 2 );
	  "mov     r1, %2       \n"  // n >> 1
	  "asr     r1, r1, 1    \n"
	  "mov     r2, %2       \n"  // n >> 2
	  "asr     r2, r2, 2    \n"
	  "add     r1, r2       \n"

	// q = q + ( q >> 4 )

	  "mov     r2, r1       \n"
	  "asr     r2, r2, 4    \n"
	  "add     r1, r2       \n"

	// q = q + ( q >> 8 )

	  "mov     r2, r1       \n"
	  "asr     r2, r2, 8    \n"
	  "add     r1, r2       \n"

	// q = q + ( q >> 16 )

	  "mov     r2, r1       \n"
	  "asr     r2, r2, 16   \n"
	  "add     r1, r2       \n"

	// q >>= 3

	  "asr     r1, r1, 3    \n"

	// r = n - q*10

	  "mov     r2, %2       \n"
	  "mov     r0, 10       \n"
	  "mov     r4, r1       \n"
	  "mul     r1, r0       \n"
	  "sub     r2, r2, r1   \n"

    // div = q + ( ( r + 6 ) >> 4 )
	  
	  "add     r2, r2, 6    \n"
	  "asr     r2, r2, 4    \n"
	  "add     r4, r4, r2   \n"

    // remu10 -------------------------------------------

	// n = ( 0x19999999*x + ( x >> 1 ) + ( x >> 3 ) ) >> 28
	  
	  "movw    r0, 0x9999   \n"
	  "movt    r0, 0x1999   \n"
	  "mul     r0, %2       \n"
	  "mov     r1, %2       \n"
	  "asr     r1, r1, 1    \n"
	  "mov     r2, %2       \n"
	  "asr     r2, r2, 3    \n"
	  "add     r0, r0, r1   \n"
	  "add     r0, r0, r2   \n"
	  "asr     r0, r0, 28   \n"

	// mod = table[ n ]

	  "mov     r2, %3       \n"
	  "add     r2, r0       \n"
	  "ldrb    r2, [ r2, 0 ] \n"
	  "str     r2, [ %1 ]"
	  
	: "=r"( div ), "=r"( mod )
	: "r"( x ), "g"( table )
	: "r4", "r2", "r1", "r0" );
	
#else

	*div = x / 10;
	*mod = x % 10;
	
#endif // CONFIG_SLAVE_FAST_MATH
}
