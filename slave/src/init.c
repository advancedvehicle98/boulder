#include <slave/state.h>

#include <stdbool.h>


boulder_init_error_t
slave_init( __STATE boulder_slave_state_t *s )
{

	// настройка CAN-шины
	
	{
		can_init_error_t init_error;
		can_state_t *can = &( s->can );

		do {
			init_error = slave_can_init( can );
		} while ( init_error != CAN_INIT_SUCCESS );
	}
	
	return SLAVE_INIT_SUCCESS;
}


void
slave_init_log_and_halt( __IN const boulder_init_error_t e )
{
	while ( true ) { }
}
