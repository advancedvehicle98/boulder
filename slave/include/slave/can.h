#ifndef __BOULDER_SLAVE_CAN_H
#define __BOULDER_SLAVE_CAN_H


#include <common/config.h>
#include <common/defines.h>
#include <slave/queue.h>
#include <soc/can.h>

#include <canard.h>
#include <dsdl/include/dronecan_msgs.h>


typedef enum {
	CAN_INIT_SUCCESS,
	CAN_INIT_TIMEOUT
} can_init_error_t;


typedef struct _can_state_t {
	uint16_t error_count;
	
	// вот эти штуки определяются в soc/<префикс soc>/include/can.h
	can_soc_state_t soc;

	buffer_queue_t rx_queue;
	uint8_t rx_queue_buffer[ CONFIG_SLAVE_CAN_RX_QUEUE_LEN ];

	// canard -----------------------------------
	CanardInstance canard;
	uint8_t canard_pool[ CONFIG_CANARD_SLAVE_POOL_SIZE ];

	// dronecan ---------------------------------
	struct uavcan_protocol_NodeStatus node_status;
	// берём размер самого большого сообщения, которое
	// будет использоваться
	uint8_t response_buffer[ UAVCAN_PROTOCOL_GETNODEINFO_RESPONSE_MAX_SIZE ];
} can_state_t;


// нужно для rx-очереди, чтобы сразу время хранить
typedef struct _can_message_t {
	CanardCANFrame frame;
	uint64_t ts;
} can_message_t;


// обработчики для dronecan

void slave_can_handle_get_node_info( __STATE CanardInstance   *canard,
									 __IN    CanardRxTransfer *rx );

// src/can/init.c
__COLD can_init_error_t slave_can_init( __STATE can_state_t *s );
// src/can/on_receive.c
__HOT void slave_can_on_receive( __STATE CanardInstance   *canard,
								 __IN    CanardRxTransfer *rx );
// src/can/process_messages.c
__HOT void slave_can_process_messages( __STATE can_state_t *s );
// src/can/should_accept_transfer.c
__HOT bool slave_can_should_accept_transfer( __IN const CanardInstance     *canard,
											 __IN       uint64_t           *out_data_type_sig,
											 __IN       uint16_t            data_type_id,
											 __IN       CanardTransferType  trx_type,
											 __IN       uint8_t             src_id );
// soc/<soc>/can/init.c
__COLD can_init_error_t slave_soc_can_init( __STATE can_soc_state_t *s );


#endif // ! __BOULDER_SLAVE_CAN_H
