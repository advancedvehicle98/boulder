#ifndef __BOULDER_SLAVE_RAM_H
#define __BOULDER_SLAVE_RAM_H


#include <common/defines.h>

#include <stddef.h>
#include <stdint.h>


// src/ram/copy.c
__HOT uint8_t slave_ram_copy( __OUT uint8_t * const dest,
							  __IN  uint8_t * const src,
							  __IN  const size_t    size );

__HOT uint8_t slave_ram_copy_reverse( __OUT uint8_t * const dest,
									  __IN  uint8_t * const src,
									  __IN  const size_t    size );

__HOT char * slave_ram_copy_string( __OUT char * const dest,
									__IN  char * const src,
									__IN  const size_t max_size );
	
// src/ram/format_string.c
__HOT size_t slave_ram_format_string( __OUT char   *out_str,
									  __IN  void   *__vargs__,
									  __IN  size_t  max_size );


#endif // ! __BOULDER_SLAVE_RAM_H
