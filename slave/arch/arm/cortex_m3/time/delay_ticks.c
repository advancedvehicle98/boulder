#include <slave/time.h>

#include <soc/arch_defs.h>
#include <core_cm3.h>
#include <system_ARMCM3.h>


// https://www.rotr.info/electronics/mcu/arm_systick.htm


#define _CAL_TICKS 500


static uint32_t _g_cal_dt = 0;


void
slave_arch_delay_ticks( const uint32_t ticks )
{	
	uint32_t t0 = SysTick->VAL;
	uint32_t t_past = t0;
	uint32_t dt = ticks;

	if ( dt <= _g_cal_dt ) return;

	dt -= _g_cal_dt;

	uint32_t t;
	
	do {
		t = SysTick->VAL;
		if ( t >= t_past ) t0 += SysTick->LOAD + 1;
		t_past = t;
	} while ( t0 - t < dt );
}


void
slave_arch_delay_ticks_calibrate( void )
{
	// сбрасываем счётчик -----------------------
	
	uint32_t load = SysTick->LOAD;

	SysTick->LOAD = 0xFFFFFF;
	SysTick->VAL  = 0;

	// отключаем прерывания ---------------------
	
	uint32_t irq_mask = __get_PRIMASK();
	__disable_irq();

	// сохраняем разность для калибровки ---------
	
	uint32_t t0 = SysTick->VAL;

	slave_arch_delay_ticks( _CAL_TICKS );

	_g_cal_dt = ( t0 - SysTick->VAL ) - _CAL_TICKS;

	// восстанавливаем счётчик ------------------

	SysTick->LOAD = load;
	SysTick->VAL  = 0;

	// включаем прерывания ----------------------

	__set_PRIMASK( irq_mask );
	__enable_irq();
}
