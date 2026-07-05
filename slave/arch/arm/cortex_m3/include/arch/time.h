#ifndef __BOULDER_ARM_CORTEX_M3_TIME_H
#define __BOULDER_ARM_CORTEX_M3_TIME_H


#include <soc/arch_defs.h>
#include <core_cm3.h>
#include <system_ARMCM3.h>

#include <stdint.h>


extern uint64_t _ticks;


uint32_t _micros( void );


static inline void
slave_arch_delay_ms( const uint32_t ms )
{
	slave_arch_delay_ticks( 8 * ms * CONFIG_ARM_CORTEX_M3_SYSTICK_CALIB );
}


static inline uint32_t
slave_arch_millis( void )
{
	return _ticks;
}


static inline uint32_t
slave_arch_micros( void )
{
	return _micros();
}


#endif // ! __BOULDER_ARM_CORTEX_M3_TIME_H
