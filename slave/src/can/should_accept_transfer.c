#include <slave/can.h>


bool
slave_can_should_accept_transfer( __IN const CanardInstance     *canard,
								  __IN       uint64_t           *out_data_type_sig,
								  __IN       uint16_t            data_type_id,
								  __IN       CanardTransferType  trx_type,
								  __IN       uint8_t             src_id )
{
	return false;
}
