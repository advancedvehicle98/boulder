#include <slave/board.h>

#include <common/defines.h>
#include <slave/soc.h>

#include <stdbool.h>


board_init_error_t
slave_board_init( __STATE soc_state_t *soc )
{
	// настройка камня ------------------------------

	uint32_t soc_init_status = slave_soc_init( soc );
	
	if ( soc_init_status != SOC_INIT_SUCCESS )
		return BOARD_INIT_FAILED_SOC_INIT;
	
	return BOARD_INIT_SUCCESS;
}


void
slave_board_init_log_and_halt( __IN const board_init_error_t e )
{
	while ( true ) { }
}
