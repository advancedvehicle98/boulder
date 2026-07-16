#include <master/state.h>


// эта штука должна всегда быть в цикле
boulder_run_status_t
master_run( __STATE boulder_master_state_t *s )
{
	if ( master_can_process_messages( &s->can ) != CAN_PROCESS_MESSAGES_SUCCESS ) {
		
	}
	
	return MASTER_RUN_CONTINUE;
}
