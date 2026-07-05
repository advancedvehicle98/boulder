#include <slave/can.h>

#include <slave/state.h>
#include <slave/time.h>


void
slave_can_handle_get_node_info( __STATE CanardInstance   *canard,
								__IN    CanardRxTransfer *rx )
{
	can_state_t *can = &boulder->can;
	struct uavcan_protocol_GetNodeInfoResponse response;

	/* can->node_status.uptime_sec = slave_ */ // WIP
}
