#ifndef __BOULDER_ATMEL_SAM3X8E_UART_H
#define __BOULDER_ATMEL_SAM3X8E_UART_H


#include <libsam/include/can.h>


typedef struct __uart_instance_t {
	Uart *iface;
} _uart_instance_t;


typedef struct _uart_soc_state_t {
	_uart_instance_t serial;
} uart_soc_state_t;


#endif // ! __BOULDER_ATMEL_SAM3X8E_UART_H
