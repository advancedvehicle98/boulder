#include <slave/state.h>
#include <soc/uart.h>


static void _uart_irq_handler( _uart_instance_t * const inst );


void
UART_Handler( void )
{
	uart_state_t *uart     = &( boulder->uart );
	uart_soc_state_t *soc  = &( uart->soc );
	_uart_instance_t *inst = &( soc->serial );
	
	_uart_irq_handler( inst );
}


// см. https://github.com/arduino/ArduinoCore-sam/blob/790ff2c852bf159787a9966bddee4d9f55352d15/cores/arduino/UARTClass.cpp#L169

void
_uart_irq_handler( _uart_instance_t * const inst )
{
	uart_state_t *uart = &( boulder->uart );
	Uart *iface = inst->iface;

	uint32_t status = iface->UART_SR;

	// если что-то получили ----------------------------------
	
	if ( soc_uart_is_data_received( status ) );
	// tbd

	// если что-то нужно ещё отправить ------------------------
	
	if ( soc_uart_is_sending_data( status ) );
	// tbd

	// обработка ошибок --------------------------------------
	
	if ( soc_uart_is_error( status ) ) iface->UART_CR |= UART_CR_RSTSTA;
}
