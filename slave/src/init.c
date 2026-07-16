#include <slave/state.h>
#include <slave/time.h>

#include <stdbool.h>


boulder_init_error_t
slave_init( __STATE boulder_slave_state_t *s )
{
	uint32_t error;
	
	// в первую очередь настраиваем UART, т.к. зачастую это единственный
	// доступный и безопасный метод дебага (не будем показывать пальцем
	// на тех разрабов, которые не добавляют нормальные пины для jtag
	// на платы)

	uart_state_t *uart = &s->uart;

	error = (uint32_t) slave_uart_init( uart );

	if ( error != UART_INIT_SUCCESS ) return SLAVE_INIT_FAILED_UART_INIT;

	// калибровка таймера -------------------------------------------------

	slave_arch_delay_ticks_calibrate();
	
	// настройка CAN-шины --------------------------------------------------
	
	can_state_t *can = &s->can;

	error = (uint32_t) slave_can_init( can );

	if ( error != CAN_INIT_SUCCESS ) return SLAVE_INIT_FAILED_CAN_INIT;

	// настройка моторов (ESC) -------------------------------------------

	motors_state_t *motors = &s->motors;

	slave_motors_init( motors );

	// настройка AHRS ----------------------------------------------------

	ahrs_state_t *ahrs = &s->ahrs;

	slave_ahrs_init( ahrs );

	// -----------------------------------------------------------------
	
	return SLAVE_INIT_SUCCESS;
}


void
slave_init_log_and_halt( __IN const boulder_init_error_t e )
{
	while ( true ) { }
}
