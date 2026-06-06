#include <slave/platform.h>

#include <slave/chip.h>

#include <stdbool.h>


platform_init_status_t
slave_platform_init( void )
{
	// будим камень -----------------------------------------------
	
	{
		chip_init_status_t init_status = slave_chip_init();

		if ( init_status != CHIP_INIT_SUCCESS ) {
			slave_chip_init_log( init_status );
			return PLATFORM_FAILED_CHIP_INIT;
		}
	}
	
	return PLATFORM_INIT_SUCCESS;
}


void
slave_platform_init_log_and_halt( __IN const platform_init_status_t e )
{
	while ( true ) { }
}
