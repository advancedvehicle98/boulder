#include <common/config.h>
#include <slave/uart.h>


uart_init_error_t
slave_uart_init( __STATE uart_state_t * const s )
{
	// делаем необходимую инициализацию на камне ---------------------
	
	uart_init_error_t soc_init_error = slave_soc_uart_init( &s->soc );

	if ( soc_init_error != UART_INIT_SUCCESS ) return soc_init_error;

	// настройка очередей ------------------------------------------

	slave_buffer_queue_init( &s->serial_rx_queue,
							 s->serial_rx_queue_buffer,
							 CONFIG_SLAVE_SERIAL_RX_QUEUE_LEN,
							 BUFFER_QUEUE_OVERWRITE );

	slave_buffer_queue_init( &s->serial_tx_queue,
							 s->serial_tx_queue_buffer,
							 CONFIG_SLAVE_SERIAL_TX_QUEUE_LEN,
							 BUFFER_QUEUE_OVERWRITE );
	
	return UART_INIT_SUCCESS;
}
