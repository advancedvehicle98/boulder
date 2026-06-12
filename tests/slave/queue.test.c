#include <slave/queue.h>

#include <cmocka.h>
#include <stdint.h>
#include <stdio.h>


static void
test_push_no_overwrite( void **state )
{
	(void) state;

	buffer_queue_t q;
	uint8_t b[ 12 ];
	
	slave_buffer_queue_init( &q, b, sizeof( b ), 0 );

	// вставка первого элемента, размером меньше самой очереди -------------
	
	uint32_t x = 0xAABBCCDD;
	
	slave_buffer_queue_push( &q, &x, sizeof( x ) );

	assert_memory_equal( &x, b, sizeof( x ) );
}


int
main( void )
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test( test_push_no_overwrite ),
	};
	
	return cmocka_run_group_tests( tests, NULL, NULL );
}
