#ifndef __BOULDER_SLAVE_MATH_H
#define __BOULDER_SLAVE_MATH_H


#include <common/defines.h>

#include <stdint.h>


// <arch>/math/divmod10.c
void slave_arch_math_divmod10( __OUT       uint32_t *div,
							   __OUT       uint32_t *mod,
							   __IN  const uint32_t  x );


#endif // ! __BOULDER_SLAVE_MATH_H
