#ifndef __BOULDER_ATMEL_SAM3X8E_STATE_H
#define __BOULDER_ATMEL_SAM3X8E_STATE_H


#include <stdint.h>


typedef struct _soc_state_t {
	uint32_t mclk; // основная частота (main clock frequency)
} soc_state_t;


#endif // ! __BOULDER_ATMEL_SAM3X8E_STATE_H
