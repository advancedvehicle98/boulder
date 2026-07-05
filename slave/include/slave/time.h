#ifndef __BOULDER_SLAVE_TIME_H
#define __BOULDER_SLAVE_TIME_H


#include <common/config.h>

#include <stdint.h>


// arch/<arch>/time/delay_ticks.c
void slave_arch_delay_ticks( const uint32_t ticks );
void slave_arch_delay_ticks_calibrate( void );


#include <arch/time.h>


#endif // ! __BOULDER_SLAVE_TIME_H
