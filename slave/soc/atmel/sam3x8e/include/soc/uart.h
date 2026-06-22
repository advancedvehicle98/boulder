#ifndef __BOULDER_ATMEL_SAM3X8E_UART_H
#define __BOULDER_ATMEL_SAM3X8E_UART_H


#include <libsam/chip.h>


typedef struct __uart_instance_t {
	Uart *iface;
} _uart_instance_t;


typedef struct _uart_soc_state_t {
	_uart_instance_t serial;
} uart_soc_state_t;


static inline bool
soc_uart_is_data_received( __IN const uint32_t status )
{
	return status & UART_SR_RXRDY;
}


static inline bool
soc_uart_is_sending_data( __IN const uint32_t status )
{
	return status & UART_SR_TXRDY;
}


static inline bool
soc_uart_is_error( __IN const uint32_t status )
{
	return status & ( UART_SR_OVRE | UART_SR_FRAME );
}


// типо публичные инлайны

static __INLINE void
slave_soc_uart_signal_for_transmit( uart_soc_state_t *uart )
{
	uart->serial.iface->UART_IER = UART_IER_TXRDY;
}


#endif // ! __BOULDER_ATMEL_SAM3X8E_UART_H
