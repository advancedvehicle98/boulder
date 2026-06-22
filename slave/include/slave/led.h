#ifndef __BOULDER_SLAVE_LED_H
#define __BOULDER_SLAVE_LED_H


#include <common/defines.h>
#include <soc/led.h>


static __INLINE void
slave_status_led_off( void )
{
	// soc/<soc>/include/led.h
	slave_soc_status_led_off();
}


static __INLINE void
slave_status_led_on( void )
{
	// soc/<soc>/include/led.h
	slave_soc_status_led_on();
}


#endif // ! __BOULDER_SLAVE_LED_H
