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

	buffer_queue_t serial_queue;
	uint8_t serial_queue_buffer[ CONFIG_SLAVE_SERIAL_QUEUE_LEN ];
} uart_state_t;


// src/uart/init.c
__COLD uart_init_error_t slave_uart_init( __STATE uart_state_t * const s );
// soc/<soc>/uart/init.c
__COLD uart_init_error_t slave_soc_uart_init( __STATE uart_soc_state_t * const s );


#endif // ! __BOULDER_SLAVE_UART_H
