#include <master/can.h>

#include <dsdl/include/dronecan_msgs.h>


bool
master_can_should_accept_transfer( __IN const CanardInstance     *canard,
								   __IN       uint64_t           *out_data_type_sig,
								   __IN       uint16_t            data_type_id,
								   __IN       CanardTransferType  trx_type,
								   __IN       uint8_t             src_id )
{
	can_state_t *can = canardGetUserReference( canard );

	if ( trx_type == CanardTransferTypeRequest )
		switch ( data_type_id ) {
			
		case UAVCAN_PROTOCOL_GETNODEINFO_ID: {
			*out_data_type_sig = UAVCAN_PROTOCOL_GETNODEINFO_REQUEST_SIGNATURE;
			return true;
		}
			
		} // switch ( data_type_id )
	
	return false;
}
