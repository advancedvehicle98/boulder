#include <slave/can.h>

#include <slave/time.h>


void
slave_can_respond( __STATE CanardInstance   *canard,
				   __STATE CanardRxTransfer *rx,
				   __IN    const uint64_t    data_type_sig,
				   __IN    const uint16_t    data_type_id,
				   __IN    const uint8_t    *payload,
				   __IN    const uint16_t    payload_size )
{
	CanardTxTransfer tx;

	tx.transfer_type       = CanardTransferTypeResponse;
	tx.data_type_signature = data_type_sig;
	tx.data_type_id        = data_type_id;
	tx.inout_transfer_id   = &rx->transfer_id;
	tx.priority            = rx->priority;
	tx.payload             = payload;
	tx.payload_len         = payload_size;

	canardRequestOrRespondObj( canard, rx->source_node_id, &tx );
}
