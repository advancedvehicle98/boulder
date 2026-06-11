#ifndef __BOULDER_MASTER_CAN_H
#define __BOULDER_MASTER_CAN_H


#include <common/config.h>
#include <common/defines.h>

#include <canard.h>
#include <dsdl/include/dronecan_msgs.h>
#include <net/if.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/ioctl.h>


#define MASTER_CAN_IF_NAME_LEN IF_NAMESIZE


typedef enum {
	CAN_INIT_SUCCESS,
	CAN_INIT_ERROR_NO_SOCKET,
	CAN_INIT_ERROR_CANNOT_BIND,
	CAN_INIT_ERROR_IOCTL,
	CAN_INIT_ERROR_WRONG_IFR_INDEX,
	CAN_INIT_ERROR_FAILED_CONFIGURATION,
	CAN_INIT_ERROR_FAILED_SOCKET_VALIDATION,
	CAN_INIT_ERROR_BITRATE_FAIL
} can_init_error_t;


typedef enum {
	CAN_SET_BITRATE_SUCCESS,
	CAN_SET_BITRATE_NO_SOCKET,
	CAN_SET_BITRATE_FAILED_LINKINFO_SET
} can_set_bitrate_error_t;


typedef struct _can_state_t {
	uint32_t bitrate;

	// socketcan
	char     if_name[ MASTER_CAN_IF_NAME_LEN ];
	uint32_t if_index;
	int32_t  socket_fd;

	// canard
	CanardInstance canard;
	uint8_t canard_pool[ CONFIG_CANARD_MASTER_POOL_SIZE ];

	// dronecan
	struct uavcan_protocol_NodeStatus node_status;
	
	bool ready;
} can_state_t;


typedef struct _can_state_args_t {
	// см. parse_args.c:_try_as_can_iface_name()
	const char *if_name;
	// см. parse_args.c:_try_as_can_bitrate()
	uint32_t    bitrate;
} can_state_args_t;


extern const char master_can_default_if_name[ MASTER_CAN_IF_NAME_LEN ];


// can/deinit.c
void master_can_deinit( __STATE can_state_t *s );
// can/init.c
can_init_error_t master_can_init( __STATE can_state_t *s, __IN const can_state_args_t *args );
void master_can_init_print_error( __IN const can_init_error_t e );
// can/on_receive.c
void master_can_on_receive( __STATE CanardInstance   *canard,
							__IN    CanardRxTransfer *rx );
// can/set_bitrate.c
can_set_bitrate_error_t master_can_set_bitrate( __IN const uint32_t if_name, __IN const uint32_t bitrate );
void master_can_set_bitrate_print_error( __IN const can_set_bitrate_error_t e );
// can/should_accept_transfer.c
bool master_can_should_accept_transfer( __IN const CanardInstance     *canard,
										__IN       uint64_t           *out_data_type_sig,
										__IN       uint16_t            data_type_id,
										__IN       CanardTransferType  trx_type,
										__IN       uint8_t             src_id );


#endif // ! __MASTER_CAN_H
