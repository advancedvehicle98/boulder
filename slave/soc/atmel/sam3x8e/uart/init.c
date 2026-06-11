#include <slave/uart.h>

#include <common/config.h>


// этот uart надо инициализировать в обязательную очередь
// добавление остальных зависит от настроек
static __COLD uart_init_error_t _serial_uart_init( __STATE _uart_instance_t * const inst,
												   __IN    const uint32_t           mclk );


uart_init_error_t
slave_soc_uart_init( __STATE uart_soc_state_t * const s )
{
	// !!! активация тактового сигнала в PMC уже сделана
	//     в slave_soc_init (soc/<soc>/init.c)

	// настраиваем uart для серийного I/O -----------------------------
	// ( это типо самый-самый из всех uart)

	uart_init_error_t serial_init_error = _serial_uart_init( &( s->serial ),
															 84000000 ); // пока так

	if ( serial_init_error != UART_INIT_SUCCESS )
		return serial_init_error;

	// для usart наверное чуть чуть отличаться будет
	// но это на потом (и пока не факт, что понадобиться)
	// хотя, если подключить лидар вместо мастера к слейву, то
	// может понадобиться
	
	return UART_INIT_SUCCESS;
}


// см. https://github.com/arduino/ArduinoCore-sam/blob/790ff2c852bf159787a9966bddee4d9f55352d15/cores/arduino/UARTClass.cpp#L49

uart_init_error_t
_serial_uart_init( __STATE _uart_instance_t * const inst,
				   __IN    const uint32_t           mclk )
{	
	Uart *serial = (Uart *) REG_UART_CR;
	
	// выключаем dma-канал для uart ----------------------------

	serial->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;

	// сброс и отключение трансмиттера и ресивера --------------------

	serial->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX
		            | UART_CR_RXDIS | UART_CR_TXDIS;

	// настройка режима 8n1 ------------------------------------

	serial->UART_MR = US_MR_CHRL_8_BIT
		            | US_MR_NBSTOP_1_BIT
		            | UART_MR_PAR_NO
		            | UART_MR_CHMODE_NORMAL;

	// настройка бода ----------------------------------------------

	serial->UART_BRGR = ( mclk / CONFIG_SLAVE_SERIAL_BAUD ) >> 4;
	
	// включаем соответствующие прерывания ----------------------------

	serial->UART_IDR = 0xFFFFFFFF;
	serial->UART_IER = UART_IER_RXRDY | UART_IER_OVRE | UART_IER_FRAME;

	NVIC_EnableIRQ( UART_IRQn );

	// теперь можно включить трансмиттер с ресивером -------------------

	serial->UART_CR = UART_CR_RXEN | UART_CR_TXEN;
	
	// конец -------------------------------------------------
	
	inst->iface = serial;
	
	return UART_INIT_SUCCESS;
}
