#include <slave/state.h>

#include <slave/time.h>
#include <common/config.h>

#ifdef CONFIG_USE_LED_STATUS
#   include <slave/led.h>
#endif

#include <stdbool.h>


boulder_run_status_t
slave_run( __STATE boulder_slave_state_t *s )
{
	if ( slave_can_process_messages( &s->can ) != CAN_PROCESS_MESSAGES_SUCCESS ) {
		
	}

	slave_ahrs_update( &s->ahrs );
	slave_range_update( &s->range );
	
	slave_motors_update( &s->motors );
	
#ifdef CONFIG_USE_LED_STATUS
	static uint8_t led_status = 0;

	if ( ! led_status ) slave_status_led_off();
	else                slave_status_led_on();

	led_status ^= 1;
#endif
	
	return SLAVE_RUN_CONTINUE;
}
