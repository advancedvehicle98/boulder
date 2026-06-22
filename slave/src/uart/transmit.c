#include <slave/uart.h>


size_t
slave_uart_transmit( __STATE uart_state_t * const  s,
					 __IN    const void           *data,
					 __IN    const size_t          size )
{
	slave_buffer_queue_push( &s->serial_tx_queue, data, size );
	slave_soc_uart_signal_for_transmit( &s->soc );
	return size;
}
