#include <slave/ram.h>


uint8_t
slave_ram_copy( __OUT uint8_t * const dest,
				__IN  uint8_t * const src,
				__IN  const size_t    size )
{
	uint8_t *dest8 = dest, *src8 = src;
	size_t size_remaining;

	if ( size >= 4 ) {
		uint32_t *dest32 = (uint32_t *) dest,
			     *src32  = (uint32_t *) src;
		
		while ( (uint8_t *) dest32 < dest + size ) *dest32++ = *src32++;

		size_remaining = src - ( (uint8_t *) src32 ) + 4;
	
		if ( size_remaining ) {
			src8  = ( (uint8_t *) src32 )  - 4;
			dest8 = ( (uint8_t *) dest32 ) - 4;
		}
	}
	else
		size_remaining = size;

	switch ( size_remaining ) {
	case 3: *dest8++ = *src8++;
	case 2: *dest8++ = *src8++;
	case 1: *dest8++ = *src8++;
	}
}


uint8_t
slave_ram_copy_reverse( __OUT uint8_t * const dest,
						__IN  uint8_t * const src,
						__IN  const size_t    size )
{
	uint8_t *src_last = src + size - 1;
	
	uint8_t *dest8 = dest, *src8 = src_last;
	size_t size_remaining;

	if ( size >= 4 ) {
		src_last -= 3;
		
		uint32_t *dest32 = (uint32_t *) dest,
			     *src32  = (uint32_t *) src_last;
		
		while ( (uint8_t *) dest32 < dest + size ) *dest32++ = *src32--;

		size_remaining = src_last - ( (uint8_t *) src32 ) + 4;
	
		if ( size_remaining ) {
			src8  = ( (uint8_t *) src32  ) + 4;
			dest8 = ( (uint8_t *) dest32 ) - 4;
		}
	}
	else
		size_remaining = size;

	switch ( size_remaining ) {
	case 3: *dest8++ = *src8--;
	case 2: *dest8++ = *src8--;
	case 1: *dest8++ = *src8--;
	}
}
