#include <common/defines.h>
#include <common/config.h>
#include <slave/board.h>
#include <slave/state.h>
#include <slave/uart.h>

#include <stdbool.h>
#include <stdint.h>


boulder_slave_state_t *boulder;


void
delay ( volatile uint32_t time )
{
	while ( time-- ) __asm__ ( "nop" );
}


__NORETURN void
main( void )
{
	
	boulder_slave_state_t state;

	boulder = &state;

/* 	// аппаратная инициализация -------------------------------------- */
	
	board_init_error_t board_init_status = slave_board_init( &( state.soc ) );

	if ( board_init_status != BOARD_INIT_SUCCESS )
		slave_board_init_log_and_halt( board_init_status );
	
	// инициализация автопилота --------------------------------------
	
	boulder_init_error_t init_status = slave_init( &state );

	if ( init_status != SLAVE_INIT_SUCCESS )
		slave_init_log_and_halt( init_status );

#ifdef CONFIG_SLAVE_SERIAL_DEBUG_MSGS
	const char _init_uart_str[] = "init success";
	slave_uart_transmit( &( state.uart ), _init_uart_str, sizeof( _init_uart_str ) );
#endif

	// рантайм ---------------------------------------------------
	
	boulder_run_status_t run_status;
		
	do {
		run_status = slave_run( &state );
	} while ( run_status == SLAVE_RUN_CONTINUE );

	slave_handle_exit( &state, run_status );
}
