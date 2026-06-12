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
	assert_ptr_equal( q.tail, &( b[ 4 ] ) );
	
	assert_uint_equal( q.flags & BUFFER_QUEUE_IS_EMPTY, 0 );

	// вставка второго элемента, размером меньше оставшегося места в очереди -------------
	
	uint32_t y = 0xEEEEFFFF;
	slave_buffer_queue_push( &q, &y, sizeof( y ) );
	assert_memory_equal( &y, &( b[ 4 ] ), sizeof( y ) );
	assert_ptr_equal( q.tail, &( b[ 8 ] ) );

	// вставка тертьего элемента, размером меньше оставшегося места в очереди -------------

	uint8_t z[] = { 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0xAA };

	bool status = slave_buffer_queue_push( &q, z, sizeof( z ) );
	assert_true( status );
	assert_memory_not_equal( &z, &( b[ 8 ] ), sizeof( z )-2 );
	assert_ptr_equal( q.tail, &( b[ 8 ] ) );

	// заполнение очереди целиком -------------------------------
	
	status = slave_buffer_queue_push( &q, z, sizeof( z )-2 );
	assert_false( status );
	assert_memory_equal( &z, &( b[ 8 ] ), sizeof( z )-2 );
	assert_ptr_equal( q.tail, b );
	assert_uint_equal( q.flags & BUFFER_QUEUE_IS_FULL, BUFFER_QUEUE_IS_FULL );
}


int
main( void )
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test( test_push_no_overwrite ),
	};
	
	return cmocka_run_group_tests( tests, NULL, NULL );
}
