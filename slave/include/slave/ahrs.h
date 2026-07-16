#ifndef __BOULDER_SLAVE_AHRS_H
#define __BOULDER_SLAVE_AHRS_H


#include <common/defines.h>


typedef struct _ahrs_state_t {
	
} ahrs_state_t;


// src/ahrs/init.c
__COLD void slave_ahrs_init( __STATE ahrs_state_t * const s );

// src/ahrs/update.c
__HOT void slave_ahrs_update( __STATE ahrs_state_t * const s );


#endif // ! __BOULDER_SLAVE_AHRS_H
