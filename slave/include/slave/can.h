#ifndef __BOULDER_SLAVE_CAN_H
#define __BOULDER_SLAVE_CAN_H


#include <common/config.h>
#include <common/defines.h>
#include <soc/can.h>

#include <canard.h>


typedef enum {
	CAN_INIT_SUCCESS,
	CAN_INIT_TIMEOUT
} can_init_error_t;


typedef struct _can_state_t {
	uint32_t bitrate;
	
	// вот эти штуки определяются в soc/<префикс soc>/include/can.h
	can_soc_state_t soc;
	
	CanardInstance canard;
	uint8_t canard_pool[ CONFIG_CANARD_SLAVE_POOL_SIZE ];
	
	bool ready;
} can_state_t;


// src/can/init.c
can_init_error_t slave_can_init( __STATE can_state_t *s );
// src/can/on_receive.c
void slave_can_on_receive( __STATE CanardInstance   *canard,
						   __IN    CanardRxTransfer *rx );
// soc/<soc>/can/init.c
can_init_error_t slave_soc_can_init( __STATE can_soc_state_t *s );
// src/can/should_accept_transfer.c
bool slave_can_should_accept_transfer( __IN const CanardInstance     *canard,
									   __IN       uint64_t           *out_data_type_sig,
									   __IN       uint16_t            data_type_id,
									   __IN       CanardTransferType  trx_type,
									   __IN       uint8_t             src_id );


#endif // ! __BOULDER_SLAVE_CAN_H
