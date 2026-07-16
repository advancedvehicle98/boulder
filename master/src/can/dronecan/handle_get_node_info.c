#include <master/dronecan.h>

#include <master/state.h>

#include <stdint.h>
#include <string.h>
#include <time.h>


void
master_dronecan_handle_get_node_info( __STATE CanardInstance   *canard,
									  __IN    CanardRxTransfer *rx )
{
	can_state_t *can = &boulder->can;
	struct uavcan_protocol_GetNodeInfoResponse response;

	struct timespec tspec;

	clock_gettime( CLOCK_MONOTONIC, &tspec );
	can->node_status.uptime_sec = tspec.tv_sec/1000U;

	response.status = can->node_status;

	memcpy( response.name.data,
			CONFIG_DRONECAN_SLAVE_NODE_NAME,
			sizeof( CONFIG_DRONECAN_SLAVE_NODE_NAME ) );

	uint16_t response_size = uavcan_protocol_GetNodeInfoResponse_encode( &response,
																		 can->response_buffer );

	master_can_respond( canard, rx,
						UAVCAN_PROTOCOL_GETNODEINFO_SIGNATURE,
						UAVCAN_PROTOCOL_GETNODEINFO_ID,
						can->response_buffer,
						response_size );
}
