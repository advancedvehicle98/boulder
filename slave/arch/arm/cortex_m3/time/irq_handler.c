#include <slave/time.h>


uint64_t _ticks = 0;


void
SysTick_Handler( void )
{
	++_ticks;
}
