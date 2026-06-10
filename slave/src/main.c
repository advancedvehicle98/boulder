#include <slave/board.h>
#include <slave/state.h>

#include <common/defines.h>

#include <stdbool.h>
#include <stdint.h>


__NORETURN void
main( void )
{
	boulder_slave_state_t state;

	// аппаратная инициализация --------------------------------------
	
	board_init_error_t board_init_status = slave_board_init( &( state.soc ) );

	if ( board_init_status != BOARD_INIT_SUCCESS )
		slave_board_init_log_and_halt( board_init_status );
	
	// инициализация автопилота --------------------------------------
	
	boulder_init_error_t init_status = slave_init( &state );

	if ( init_status != SLAVE_INIT_SUCCESS )
		slave_init_log_and_halt( init_status );

	// рантайм ---------------------------------------------------
	
	boulder_run_status_t run_status;
		
	do {
		run_status = slave_run( &state );
	} while ( run_status == SLAVE_RUN_CONTINUE );

	slave_handle_exit( &state, run_status );
}
