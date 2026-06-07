#ifndef __BOULDER_SLAVE_CAN_H
#define __BOULDER_SLAVE_CAN_H


#include <slave/platform.h>

#include <common/defines.h>


typedef enum {
	CAN_INIT_SUCCESS
} can_init_error_t;


typedef struct _can_state_t {
	// вот эти штуки определяются в include/chips/<чип>/can.h
	// а из какого заголовка берётся решает уже plarform.h
	can_chip_specific_state_t specifics;
	bool ready;
} can_state_t;


can_init_error_t slave_can_init( __STATE can_state_t *s );
// chips/<чип>/can/init.c
can_init_error_t slave_chip_can_init( __STATE can_chip_specific_state_t *s );


#endif // ! __BOULDER_SLAVE_CAN_H
