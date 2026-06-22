#include <slave/uart.h>

#include <cmocka.h>


uart_state_t _uart_;


static void test_uart_transmit( void* *state );


int
main( void )
{
	slave_uart_init( &_uart_ );
	
	const struct CMUnitTest tests[] = {
		cmocka_unit_test( test_uart_transmit )
	};

	return cmocka_run_group_tests( tests, NULL, NULL );
}


void
test_uart_transmit( void* *state )
{
	unsigned char data[] = "Hello from UART";
	size_t size = sizeof( data );

	assert_uint_equal( slave_uart_transmit( &_uart_, data, size ), size );
	assert_memory_equal( data, _uart_.serial_tx_queue_buffer, size );

	uint8_t tx_byte;
	size_t i = 0;

	while ( ! slave_buffer_queue_is_empty( &_uart_.serial_tx_queue ) ) {
		assert_true( i < size );
		
		tx_byte = slave_buffer_queue_pop_byte( &_uart_.serial_tx_queue );
		assert_uint_equal( tx_byte, data[ i++ ] );
	}
}
