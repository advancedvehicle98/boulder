#ifndef __BOULDER_ARM_CORTEX_M3_TIME_H
#define __BOULDER_ARM_CORTEX_M3_TIME_H


#include <stdint.h>
#include <time.h>
#include <unistd.h>


static inline void
slave_arch_delay_ms( const uint32_t ms )
{
	usleep( ms );
}


static inline uint32_t
slave_arch_millis( void )
{
	struct timespec ts;
	clock_gettime( CLOCK_MONOTONIC, &ts );
	return ts.tv_sec * 1000 + ts.tv_nsec/1000000;
}


static inline uint32_t
slave_arch_micros( void )
{
	struct timespec ts;
	clock_gettime( CLOCK_MONOTONIC, &ts );
	return ts.tv_sec * 1000000 + ts.tv_nsec/1000;
}


#endif // ! __BOULDER_ARM_CORTEX_M3_TIME_H
