#ifndef __BOULDER_SLAVE_CAN_H
#define __BOULDER_SLAVE_CAN_H


#include <soc/can.h>

#include <common/defines.h>


typedef enum {
	CAN_INIT_SUCCESS,
	CAN_INIT_TIMEOUT
} can_init_error_t;


typedef struct _can_state_t {
	uint32_t bitrate;
	// вот эти штуки определяются в soc/<префикс soc>/include/can.h
	can_soc_state_t soc;
	
	bool ready;
} can_state_t;


// src/can/init.c
can_init_error_t slave_can_init( __STATE can_state_t *s );
// soc/<soc>/can/init.c
can_init_error_t slave_soc_can_init( __STATE can_soc_state_t *s );


#endif // ! __BOULDER_SLAVE_CAN_H
