#include <slave/math.h>


void
slave_arch_math_divmod10( __OUT       uint32_t *div,
						  __OUT       uint32_t *mod,
						  __IN  const uint32_t  x )
{
	*div = x / 10;
	*mod = x % 10;
}
