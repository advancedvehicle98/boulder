#include <slave/time.h>

#include <common/config.h>


// см. https://github.com/arduino/ArduinoCore-sam/blob/790ff2c852bf159787a9966bddee4d9f55352d15/cores/arduino/wiring.c#L35

uint32_t
_micros( void )
{
	uint32_t t, t2, p, p2, c, c2;

	t2 = SysTick->VAL;
	p2 =   ( SCB->ICSR  & SCB_ICSR_PENDSTSET_Msk )
		|| ( SCB->SHCSR & SCB_SHCSR_SYSTICKACT_Msk );
	c2 = _ticks;

	do {
		t = t2; p = p2; c = c2;
		t2 = SysTick->VAL;
		p2 =   ( SCB->ICSR  & SCB_ICSR_PENDSTSET_Msk )
			|| ( SCB->SHCSR & SCB_SHCSR_SYSTICKACT_Msk );
		c2 = _ticks;
	} while ( p != p2 || c != c2 || t < t2 );

	return ( c + p )*1000
		 + ( ( ( SysTick->LOAD - t )
			   * ( 0x100000/( CONFIG_SLAVE_SOC_CLOCK_FREQUENCY/1000000 ) ) )
			 >> 20 );
}
