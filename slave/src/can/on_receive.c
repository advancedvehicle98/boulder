#include <slave/can.h>


void
slave_can_on_receive( __STATE CanardInstance   *canard,
					  __IN    CanardRxTransfer *trx )
{
	switch ( trx->data_type_id ) {

	case UAVCAN_PROTOCOL_GETNODEINFO_ID: {
		slave_can_handle_get_node_info( canard, trx );
	} break;
		
	}
}
