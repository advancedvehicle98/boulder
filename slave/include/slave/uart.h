#ifndef __BOULDER_SLAVE_UART_H
#define __BOULDER_SLAVE_UART_H


#include <common/config.h>
#include <common/defines.h>
#include <slave/queue.h>
#include <soc/uart.h>

#include <stdint.h>


typedef enum {
	UART_INIT_SUCCESS
} uart_init_error_t;


typedef struct _uart_state_t {
	// см. soc/<soc>/include/soc/uart.h
	uart_soc_state_t soc;

	buffer_queue_t serial_rx_queue;
	uint8_t serial_rx_queue_buffer[ CONFIG_SLAVE_SERIAL_RX_QUEUE_LEN ];
	buffer_queue_t serial_tx_queue;
	uint8_t serial_tx_queue_buffer[ CONFIG_SLAVE_SERIAL_TX_QUEUE_LEN ];
} uart_state_t;


// soc/<soc>/uart/init.c
__COLD uart_init_error_t slave_soc_uart_init( __STATE uart_soc_state_t * const s );

// src/uart/init.c
__COLD uart_init_error_t slave_uart_init( __STATE uart_state_t * const s );

// src/uart/printf.c
__HOT size_t slave_uart_printf( __STATE uart_state_t * const s,
								__IN    const char *         fmt, ... );

// src/uart/transmit.c
__HOT size_t slave_uart_transmit( __STATE uart_state_t * const s,
								  __IN    const void * const   data,
								  __IN    const size_t         size );


#endif // ! __BOULDER_SLAVE_UART_H
