#include <slave/state.h>

#include <stdbool.h>


boulder_init_status_t
slave_init( __STATE boulder_slave_state_t *s )
{
	return SLAVE_INIT_SUCCESS;
}


void
slave_init_log_and_halt( __IN const boulder_init_status_t e )
{
	while ( true ) { }
}
