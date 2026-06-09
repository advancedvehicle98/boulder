#include <master/state.h>

#include <stdio.h>
#include <stdlib.h>


void _handle_can_status_error( const can_init_error_t e );


uint32_t
master_init( __STATE       boulder_state_t *s,
			 __IN    const boulder_state_args_t *args )
{
	const char *log_prefix = "master_init";
	
	// инициалиация CAN-интерфейса --------------------------------------
	
	can_state_t *can_state = &( s->can );
	can_state->ready = false;
		
	can_init_error_t can_init_status = master_can_init( can_state, &( args->can ) );

	if ( _unlikely( ! can_state->ready ) ) {
		printf( "%s: Не удалось инициализировать CAN-интерфейс\n", log_prefix );
		master_can_init_print_error( can_init_status );
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}


