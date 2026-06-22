#include <slave/time.h>

#include <time.h>


void
slave_arch_delay_ticks( const uint32_t ticks )
{
	struct timespec t = { ticks/10e9, ticks%( (uint32_t) 10e9 ) };
	nanosleep( &t, NULL );
}
