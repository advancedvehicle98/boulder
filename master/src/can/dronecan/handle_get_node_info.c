#include <master/dronecan.h>


#include <stdint.h>
#include <string.h>


void
master_dronecan_handle_get_node_info( __STATE can_state_t      *can,
									  __IN    CanardRxTransfer *trx )
{
	uint8_t buffer[ UAVCAN_PROTOCOL_GETNODEINFO_RESPONSE_MAX_SIZE ];
	struct uavcan_protocol_GetNodeInfoResponse packet;

	memset( &packet, 0, sizeof( packet ) );

	
}
