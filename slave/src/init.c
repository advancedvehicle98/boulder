#include <slave/state.h>
#include <slave/time.h>

#include <stdbool.h>


boulder_init_error_t
slave_init( __STATE boulder_slave_state_t *s )
{
	// в первую очередь настраиваем UART, т.к. зачастую это единственный
	// доступный и безопасный метод дебага (не будем показывать пальцем
	// на тех разрабов, которые не добавляют нормальные пины для jtag
	// на платы)

	uart_state_t *uart = &( s->uart );

	uart_init_error_t uart_init_error = slave_uart_init( uart );

	if ( uart_init_error != UART_INIT_SUCCESS )
		return SLAVE_INIT_FAILED_UART_INIT;
	
	// настройка CAN-шины --------------------------------------------------
	
	can_state_t *can = &( s->can );

	can_init_error_t can_init_error = slave_can_init( can );

	if ( can_init_error != CAN_INIT_SUCCESS )
		return SLAVE_INIT_FAILED_CAN_INIT;

	// калибровка таймера ---------------------------------------------------

	slave_arch_delay_ticks_calibrate();
	
	return SLAVE_INIT_SUCCESS;
}


void
slave_init_log_and_halt( __IN const boulder_init_error_t e )
{
	while ( true ) { }
}
