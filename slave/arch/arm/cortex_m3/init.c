#include <slave/arch.h>

#include <common/config.h>
#include <soc/arch_defs.h>

#include <core_cm3.h>
#include <system_ARMCM3.h>


arch_init_error_t
slave_arch_init( void )
{
	// настройка SysTick
	/* if ( SysTick_Config( CONFIG_SLAVE_SOC_CLOCK_FREQUENCY/1000 ) ) */
	/* 	return ARCH_INIT_FAILED_SYSTICK_INIT; */

	return ARCH_INIT_SUCCESS;
}
