#ifndef __BOULDER_ATMEL_SAM3X8E_LED_H
#define __BOULDER_ATMEL_SAM3X8E_LED_H


#include <common/defines.h>
#include <board/pins.h>


static __INLINE void
slave_soc_status_led_off( void )
{
	BUS_PIN_LED_STATUS->PIO_CODR = 1 << PIN_LED_STATUS;
}


static __INLINE void
slave_soc_status_led_on( void )
{
	BUS_PIN_LED_STATUS->PIO_SODR = 1 << PIN_LED_STATUS;
}


#endif // ! __BOULDER_ATMEL_SAM3X8E_LED_H
