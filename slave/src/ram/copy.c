#include <slave/ram.h>

#include <common/defines.h>

#include <stdbool.h>


bool _has_zero( const uint32_t x );


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


char *
slave_ram_copy_string( __OUT char * const dest,
					   __IN  char * const src,
					   __IN  const size_t max_size )
{
	char *dest_end = dest;
	char *dest_boundary = dest + max_size;
	char *_src = src;

	// 4 == sizeof( uint32_t )
	size_t few = ( -(int32_t) dest ) % 4;

	for ( ; few; --few ) {
		char c = *_src++;
		*dest_end++ = c;
		if ( c == '\0' || dest_end == dest_boundary ) return dest_end;
	}

	uint32_t *dest_end32 = (uint32_t *) dest_end;
	uint32_t *src32 = (uint32_t *) _src;
	char *zero;
	uint32_t chars;
	
	while ( dest_end < dest_boundary ) {
		chars = *src32++;
		if ( _has_zero( chars ) ) break;
		*dest_end32++ = chars;
	}

	dest_end = (char *) dest_end32;

	while ( chars & 0xFF ) {
		*dest_end++ = (uint8_t) chars;
		chars >>= 8;
	}
	
	return dest_end;
}


// ==================================================


bool
_has_zero( const uint32_t x )
{
	// https://graphics.stanford.edu/~seander/bithacks.html#ZeroInWord
	uint32_t lsb = 0x01010101;
	uint32_t msb = 0x80808080;
	return !!( ( x - lsb ) & ~x & msb );
}
