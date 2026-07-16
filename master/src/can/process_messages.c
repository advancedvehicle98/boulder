#include <master/can.h>


can_process_messages_error_t
master_can_process_messages( __STATE can_state_t *s )
{
	/* while ( ! slave_buffer_queue_is_empty( &s->rx_queue ) ) { */
	/* 	can_message_t msg; */
	/* 	/\* slave_buffer_queue_pop( &s->rx_queue, &msg, sizeof( can_message_t ) ); *\/ */
	/* 	canardHandleRxFrame( &s->canard, &msg.frame, msg.ts ); */
	/* } */

	return CAN_PROCESS_MESSAGES_SUCCESS;
}
