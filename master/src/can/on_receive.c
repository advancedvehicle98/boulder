#include <master/can.h>
#include <master/state.h>

#include <master/dronecan.h>

#include <dsdl/include/dronecan_msgs.h>


void
master_can_on_receive( __STATE CanardInstance   *canard,
					   __IN    CanardRxTransfer *trx )
{
	can_state_t *can = &boulder->can;

	if ( trx->transfer_type == CanardTransferTypeRequest )
		switch ( trx->data_type_id ) {

		case UAVCAN_PROTOCOL_GETNODEINFO_ID: {
			master_dronecan_handle_get_node_info( canard, trx );
			return;
		}
			
		}
}
