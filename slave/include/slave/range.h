#ifndef __BOULDER_SLAVE_RANGE_H
#define __BOULDER_SLAVE_RANGE_H


#include <common/defines.h>


typedef struct _range_state_t {
	
} range_state_t;


// src/range/init.c
__COLD void slave_range_init( __STATE range_state_t * const s );

// src/range/update.c
__HOT void slave_range_update( __STATE range_state_t * const s );


#endif // ! __BOULDER_SLAVE_RANGE_H
