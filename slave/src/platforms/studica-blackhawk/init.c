#include <slave/platform.h>

#include <slave/chip.h>

#include <stdbool.h>


platform_init_error_t
slave_platform_init( void )
{
	// будим камень -----------------------------------------------
	
	{
		chip_init_error_t init_error = slave_chip_init();

		if ( init_error != CHIP_INIT_SUCCESS ) {
			slave_chip_init_log( init_error );
			return PLATFORM_FAILED_CHIP_INIT;
		}
	}
	
	return PLATFORM_INIT_SUCCESS;
}


void
slave_platform_init_log_and_halt( __IN const platform_init_error_t e )
{
	while ( true ) { }
}
