#include <slave/ram.h>

#include <stdbool.h>


size_t
slave_ram_format_string( __OUT char   *out_str,
						 __IN  void   *__vargs__,
						 __IN  size_t  max_size )
{
	size_t str_size = 0;
	char *fmt = (char *) __vargs__;
	void *arg = __vargs__ + sizeof( char * );
	char *out = out_str;

	for ( ; *fmt && str_size < max_size; ++fmt ) {
		size_t appended_size = 1;
		bool sign = false;
		
		if ( *fmt != '%' ) {
			*out++ = *fmt++;
			goto _slave_ram_format_string_continue;
		}

		switch ( *( ++fmt ) ) {

		case 'c': {
			*out++ = *( (char *) arg );
			arg += sizeof( char );
		} break;

		case 's': {
			char *str = arg;
			char *new_out = slave_ram_copy_string( out, str, max_size - str_size );
			appended_size = new_out - out;
			out = new_out;
			arg += appended_size;
		} break;

		case 'i': {
			sign = true;
		}
		case 'u': {
			
		}
			
		}
		
_slave_ram_format_string_continue:
		str_size += appended_size;
	}

	if ( str_size < max_size ) *out = '\0';
	
	return str_size;
}
