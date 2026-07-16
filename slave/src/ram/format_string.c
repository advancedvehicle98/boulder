#include <slave/ram.h>

#include <slave/math.h>

#include <stdarg.h>
#include <stdbool.h>


static inline char *_format_integer( __OUT char * const   out,
									 __IN  const uint32_t integer,
									 __IN  const size_t   size,
									 __IN  const size_t   base,
									 __IN  const bool     is_signed,
									 __IN  const size_t   max_size );


size_t
slave_ram_format_string( __OUT       char   *out_str,
						 __IN  const size_t  max_size,
						 __IN  const char   *fmt, ... )
{
	size_t str_size = 0;
	char *out = out_str;

	va_list args;
	va_start( args, fmt );

	for ( ; *fmt && str_size < max_size; ) {
		size_t appended_size = 1;
		bool sign = false;
		size_t base = 1;
		
		if ( *fmt != '%' ) {
			*out++ = *fmt++;
			goto _slave_ram_format_string_continue;
		}

		switch ( *( ++fmt ) ) {

			// символы -------------------------------
			
		case 'c': {
			*out++ = va_arg( args, int );
		} break;

			// строки ----------------------------------
			
		case 's': {
			char * const str = va_arg( args, char * const );
			char *new_out = slave_ram_copy_string( out, str, max_size - str_size );
			appended_size = new_out-1 - out;
			out = new_out-1;
		} break;

			// числа ----------------------------------
			
		case 'x': {
			base <<= 3;
		}
		case 'b': {
			base <<= 1;
			sign = true;
		}
		case 'i': {
			// т.е. тут будет true, если у нас %i,
			// и будет false, если %x или %b
			sign = ! sign;
		}
		case 'u': {
			uint32_t value = va_arg( args, int );
			size_t arg_size = 0;

			switch ( *( ++fmt )  ) {				
			default:
			case '4': arg_size += 2;
			case '2': arg_size += 1; 
			case '1': arg_size += 1;
			}

			size_t width = arg_size << 3;
			if ( arg_size != 4 )
				// здесь очень странный баг присутствует, из-за которого
				// 32-битные числа зануляются
				// поэтому тут пока if
				value &= (uint32_t) ( 1 << ( width ) ) - 1;

			// sign extend
			// https://graphics.stanford.edu/~seander/bithacks.html#VariableSignExtend
			
			if ( sign ) {
				int m = 1 << ( width - 1 );
				value = ( value ^ m ) - m;
			}
				
			char *new_out = _format_integer( out, value, arg_size, base,
											 sign, max_size - str_size );
			appended_size = new_out - out;
			out = new_out;
		}
			
		} // switch ( *( ++fmt ) )

		++fmt;
			
_slave_ram_format_string_continue:
		str_size += appended_size;
	} // for ( ; *fmt && str_size < max_size; )

	va_end( args );
	
	if ( str_size < max_size ) {
		*out = '\0';
		++str_size;
	}
	
	return str_size;
}


char *
_format_integer( __OUT char * const   out,
				 __IN  const uint32_t integer,
				 __IN  const size_t   size,
				 __IN  const size_t   base,
				 __IN  const bool     is_signed,
				 __IN  const size_t   max_size )
{
	char *str_end = out;
	char *out_boundary = out + max_size;
	uint32_t x = integer;
	size_t reverse_offset = 0;

	// добавляем минус -------------------------------
	
	if ( is_signed ) {
		if ( str_end + 1 == out_boundary ) return out_boundary;
		
		if ( (int32_t) x < 0 ) {
			*str_end++ = '-';
			x = (uint32_t) -( (int32_t) x );
			++reverse_offset;
		}
	}

	// добавляем префикс 0x/0b -----------------------
	
	if ( base == 2 || base == 16 ) {
		if ( str_end + 2 >= out_boundary ) return out_boundary;
		*str_end++ = '0';
		*str_end++ = base == 2 ? 'b' : 'x';
		reverse_offset += 2;
	}

	// копируем цифры ------------------------------

	size_t shift = 4, mask = 0xF;

	if ( base == 2 ) shift = mask = 1;

	while ( x && str_end < out_boundary ) {
		char c;
		uint32_t c32;
		
		switch ( base ) {

		case 2:
		case 16: {
			c = (char) ( x & mask );
			x >>= shift;
		} break;
			
		default: {
			slave_arch_math_divmod10( &x, &c32, x );
			c = (char) c32;
			/* c = (char) ( x % 10 ); */
			/* x /= 10; */
		}
			
		}

		c += '0';
		if ( c > '9' ) c += 'A' - '9' - 1;
		
		*str_end++ = c;
	}
	

	// разворачиваем цифры -------------------------------
	
	char *right = str_end-1;
	char *left  = out + reverse_offset;

	while ( left < right ) {
		char l = *left, r = *right;
		*left++ = r; *right-- = l;
	}
		
	return str_end;
}
