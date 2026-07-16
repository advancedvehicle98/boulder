#ifndef __BOULDER_SLAVE_MOTORS_H
#define __BOULDER_SLAVE_MOTORS_H


#include <common/defines.h>


typedef struct _motors_state_t {
	
} motors_state_t;


// src/motors/init.c
__COLD void slave_motors_init( __STATE motors_state_t * const s );

// src/motors/update.c
__HOT void slave_motors_update( __STATE motors_state_t * const s );


#endif // ! __BOULDER_SLAVE_MOTORS_H
