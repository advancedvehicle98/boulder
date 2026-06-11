#include <slave/uart.h>


uart_init_error_t
slave_uart_init( __STATE uart_state_t * const s )
{
	// делаем необходимую инициализацию на камне ---------------------
	
	uart_init_error_t soc_init_error = slave_soc_uart_init( &( s->soc ) );

	if ( soc_init_error != UART_INIT_SUCCESS ) return soc_init_error;
	
	return UART_INIT_SUCCESS;
}
