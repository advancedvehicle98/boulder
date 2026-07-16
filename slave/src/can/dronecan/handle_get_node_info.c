#include <slave/dronecan.h>

#include <slave/ram.h>
#include <slave/state.h>
#include <slave/time.h>


void
slave_can_handle_get_node_info( __STATE CanardInstance   *canard,
								__IN    CanardRxTransfer *rx )
{
	can_state_t *can = &boulder->can;
	struct uavcan_protocol_GetNodeInfoResponse response;

	can->node_status.uptime_sec = slave_arch_millis()/1000U;

	response.status = can->node_status;

	slave_ram_copy( response.name.data,
					(const unsigned char * const) CONFIG_DRONECAN_SLAVE_NODE_NAME,
					sizeof( CONFIG_DRONECAN_SLAVE_NODE_NAME ) );

	uint16_t response_size = uavcan_protocol_GetNodeInfoResponse_encode( &response,
																		 can->response_buffer );

	slave_can_respond( canard, rx,
					   UAVCAN_PROTOCOL_GETNODEINFO_SIGNATURE,
					   UAVCAN_PROTOCOL_GETNODEINFO_ID,
					   can->response_buffer,
					   response_size );
}
