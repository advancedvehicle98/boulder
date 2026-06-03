#include <master/state.h>

#include <stdio.h>
#include <stdlib.h>


void _handle_can_status_error( const can_init_error_t e );


uint32_t
master_init( __STATE       boulder_state_t *s,
			 __IN    const boulder_state_args_t *args )
{
	// инициалиация CAN-интерфейса
	
	can_state_t *can_state = &( s->can );
	
	can_state->ready = false;
	can_init_error_t can_init_status = master_can_init( can_state, &( args->can ) );

	if ( ! can_state->ready ) {
		puts( "Не удалось инициализировать CAN-интерфейс" );
		_handle_can_status_error( can_init_status );
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}


void
_handle_can_status_error( const can_init_error_t e )
{
	switch ( e ) {

	case CAN_INIT_ERROR_NO_SOCKET:
		puts( "Не удалось открыть сокет" ); break;

	case CAN_INIT_ERROR_CANNOT_BIND:
		puts( "Не удалось привязать сокет к CAN-интерфейсу" );
			
	}
}
