#include <slave/uart.h>

#include <slave/ram.h>


size_t
slave_uart_printf( __STATE uart_state_t * const s,
				   __IN    const char *         fmt, ... )
{
	char str[ CONFIG_SLAVE_SERIAL_RX_QUEUE_LEN ];
	void *vargs = &fmt;
	size_t out_size = slave_ram_format_string( str, vargs );
	return slave_uart_transmit( s, str, out_size );
}
