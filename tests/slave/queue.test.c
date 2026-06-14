#include <slave/queue.h>

#include <cmocka.h>
#include <stdint.h>
#include <stdio.h>


static void test_byte_fifo( void **state );
static void test_byte_lifo( void **state );
static void test_push_no_overwrite( void **state );
static void test_push_overwrite( void **state );
static void test_pop_fifo( void **state );
static void test_pop_lifo( void **state );


int
main( void )
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test( test_push_no_overwrite ),
		cmocka_unit_test( test_push_overwrite ),
		cmocka_unit_test( test_pop_fifo ),
		cmocka_unit_test( test_pop_lifo ),
		cmocka_unit_test( test_byte_fifo ),
		cmocka_unit_test( test_byte_lifo )
	};
	
	return cmocka_run_group_tests( tests, NULL, NULL );
}


void
test_byte_fifo( void **state )
{
	(void) state;
	
	uint8_t buf[ 8 ];
	buffer_queue_t q;

	slave_buffer_queue_init( &q, buf, sizeof( buf ), BUFFER_QUEUE_OVERWRITE );

	// перезаполенение -------------------------------------
	
	bool status = false;
	uint8_t verify_push[] = { 8, 9, 2, 3, 4, 5, 6, 7 };

	for ( uint8_t x = 0; x < 10; ++x )
		status |= slave_buffer_queue_push_byte( &q, x );

	assert_true( status );
	assert_true( slave_buffer_queue_is_full( &q ) );
	
	assert_memory_equal( verify_push, buf, sizeof( verify_push ) );
	assert_ptr_equal( q.tail, &buf[ 2 ] );
	assert_ptr_equal( q.head, q.tail );

	// извлечение -----------------------------------

	uint8_t popped;
	
	for ( int i = 0; i < 6; ++i ) {
		popped = slave_buffer_queue_pop_byte( &q );
		assert_uint_equal( verify_push[ i + 2 ], popped );
	}

	uint8_t verify_pop[ 2 ] = { 8, 9 };
	
	assert_memory_equal( verify_pop, q.head, sizeof( verify_pop ) );

	assert_uint_equal( 8, slave_buffer_queue_pop_byte( &q ) );
	assert_uint_equal( 9, slave_buffer_queue_pop_byte( &q ) );

	assert_true( slave_buffer_queue_is_empty( &q ) );
}


void
test_byte_lifo( void **state )
{
	(void) state;
	
	uint8_t buf[ 8 ];
	buffer_queue_t q;

	slave_buffer_queue_init( &q, buf, sizeof( buf ),
							 BUFFER_QUEUE_OVERWRITE | BUFFER_QUEUE_LIFO );

	// перезаполенение -------------------------------------
	// (ассерты были в fifo)
	
	uint8_t verify_push[] = { 8, 9, 2, 3, 4, 5, 6, 7 };

	for ( uint8_t x = 0; x < 10; ++x )
		slave_buffer_queue_push_byte( &q, x );

	// извлечение -----------------------------------

	uint8_t popped;
	
	for ( int i = 1; i >= 0; --i ) {
		popped = slave_buffer_queue_pop_byte( &q );
		assert_uint_equal( verify_push[ i ], popped );
	}
	
	for ( int i = 7; i >= 6; --i ) {
		popped = slave_buffer_queue_pop_byte( &q );
		assert_uint_equal( verify_push[ i ], popped );
	}

	uint8_t verify_pop[ 4 ] = { 2, 3, 4, 5 };
	
	assert_memory_equal( verify_pop, q.head, sizeof( verify_pop ) );
	
	assert_uint_equal( 5, slave_buffer_queue_pop_byte( &q ) );
	assert_uint_equal( 4, slave_buffer_queue_pop_byte( &q ) );
	assert_uint_equal( 3, slave_buffer_queue_pop_byte( &q ) );
	assert_uint_equal( 2, slave_buffer_queue_pop_byte( &q ) );

	assert_true( slave_buffer_queue_is_empty( &q ) );		
}

	
void
test_pop_fifo( void **state )
{
	(void) state;

	buffer_queue_t q;
	uint8_t buf[ 64 ];
	size_t size;

	slave_buffer_queue_init( &q, buf, sizeof( buf ), BUFFER_QUEUE_OVERWRITE );

	// проверка на одном элементе --------------------------------

	uint32_t x = 0x04030201;
	uint32_t y;

	slave_buffer_queue_push( &q, &x, sizeof( x ) );
	size = slave_buffer_queue_pop( &q, &y, sizeof( y ) );
	
	assert_ptr_equal( q.head, q.tail );
	assert_ptr_equal( q.head, buf + sizeof( x ) );
	
	assert_uint_equal( x, y );
	assert_uint_equal( q.flags & BUFFER_QUEUE_IS_EMPTY, BUFFER_QUEUE_IS_EMPTY );
	assert_uint_equal( size, sizeof( y ) );

	// заполнение очереди хернёй -------------------------------------

	for ( int i = 0; i < sizeof( buf ); i += 4, x += 0x04040404 )
		slave_buffer_queue_push( &q, &x, sizeof( x ) );

	assert_uint_equal( q.flags & BUFFER_QUEUE_IS_FULL, BUFFER_QUEUE_IS_FULL );
	
	// вытаскиваем крайний элемент -----------------------------------

	size = slave_buffer_queue_pop( &q, &y, sizeof( y ) );

	assert_ptr_equal( q.head, buf + sizeof( y )*2 );

	assert_uint_equal( size, sizeof( y ) );
	assert_uint_equal( 0x04030201, y );
	assert_uint_equal( q.flags & BUFFER_QUEUE_IS_EMPTY, 0 );

	// с перезаполнением ----------------------------------------------

	uint32_t z[] = { 0xFFFFFFFF, 0xEEEEEEEE };

	bool status = slave_buffer_queue_push( &q, z, sizeof( z ) );

	// на всякий
	assert_true( status );
	
	assert_ptr_equal( q.head, buf + sizeof( y )*3 );
	assert_ptr_equal( q.head, q.tail );
	
	size = slave_buffer_queue_pop( &q, &y, sizeof( y ) );

	assert_ptr_equal( q.head, buf + sizeof( y )*4 );
	
	assert_uint_equal( size, sizeof( y ) );
	assert_uint_equal( 0x0C0B0A09, y );
	assert_uint_equal( q.flags & BUFFER_QUEUE_IS_EMPTY, 0 );

	// вытаскиваем большую часть очереди (чтобы пустить её по кругу) -------

	uint8_t w[ 64 - ( 4 - 1 )*sizeof( uint32_t ) ];

	size = slave_buffer_queue_pop( &q, w, sizeof( w ) );

	assert_uint_equal( size, sizeof( w ) );
	
	assert_ptr_equal( q.head, q.start + sizeof( uint32_t ) );
	assert_ptr_equal( q.tail, buf + sizeof( y )*3 );

	// вытаскиваем всё остальное ---------------------------------------

	uint8_t v[ 64 - sizeof( w ) ];

	size = slave_buffer_queue_pop( &q, v, sizeof( v ) );

	assert_uint_equal( size, sizeof( v ) - sizeof( 0x04 ) );
	assert_uint_equal( q.flags & BUFFER_QUEUE_IS_EMPTY, BUFFER_QUEUE_IS_EMPTY );
	
	assert_ptr_equal( q.tail, q.head );
}


void
test_pop_lifo( void **state )
{
	(void) state;

	buffer_queue_t q;
	uint8_t buf[ 64 ];
	size_t size;

	slave_buffer_queue_init( &q, buf, sizeof( buf ),
							 BUFFER_QUEUE_OVERWRITE | BUFFER_QUEUE_LIFO );

	// проверка на одном элементе --------------------------------

	uint32_t x = 0x04030201;
	uint32_t y;

	slave_buffer_queue_push( &q, &x, sizeof( x ) );
	size = slave_buffer_queue_pop( &q, &y, sizeof( y ) );
	
	assert_ptr_equal( q.head, q.tail );
	assert_ptr_equal( q.tail, buf );
	
	assert_uint_equal( x, y );
	assert_uint_equal( q.flags & BUFFER_QUEUE_IS_EMPTY, BUFFER_QUEUE_IS_EMPTY );
	assert_uint_equal( size, sizeof( y ) );

	// заполнение очереди хернёй -------------------------------------

	for ( int i = 0; i < sizeof( buf ); i += 4, x += 0x04040404 )
		slave_buffer_queue_push( &q, &x, sizeof( x ) );

	assert_uint_equal( q.flags & BUFFER_QUEUE_IS_FULL, BUFFER_QUEUE_IS_FULL );
	
	// вытаскиваем крайний элемент -----------------------------------

	size = slave_buffer_queue_pop( &q, &y, sizeof( y ) );

	assert_ptr_equal( q.tail, q.end - sizeof( uint32_t ) );

	assert_uint_equal( size, sizeof( y ) );
	assert_uint_equal( 0x04030201 + 0x04040404*15, y );
	assert_uint_equal( q.flags & BUFFER_QUEUE_IS_EMPTY, 0 );

	// с перезаполнением ----------------------------------------------

	uint32_t z[] = { 0xFFFFFFFF, 0xEEEEEEEE };

	bool status = slave_buffer_queue_push( &q, z, sizeof( z ) );

	// на всякий
	assert_true( status );
	
	assert_ptr_equal( q.head, buf + sizeof( y ) );
	assert_ptr_equal( q.head, q.tail );
	
	size = slave_buffer_queue_pop( &q, &y, sizeof( y ) );

	assert_ptr_equal( q.tail, buf );
	
	assert_uint_equal( size, sizeof( y ) );
	assert_uint_equal( z[ 1 ], y );
	assert_uint_equal( q.flags & BUFFER_QUEUE_IS_EMPTY, 0 );
	
	// пускаем очередь по кругу -------==========================

	uint8_t w[ 64 - 3*sizeof( uint32_t ) ];

	size = slave_buffer_queue_pop( &q, w, sizeof( w ) );

	assert_uint_equal( size, sizeof( w ) );
	
	assert_ptr_equal( q.tail, q.start + 3*sizeof( uint32_t ) );

	// вытаскиваем всё остальное ---------------------------------------

	uint8_t v[ 64 - sizeof( w ) ];

	size = slave_buffer_queue_pop( &q, v, sizeof( v ) );

	assert_uint_equal( size, sizeof( v ) - 0x04 );
	assert_uint_equal( q.flags & BUFFER_QUEUE_IS_EMPTY, BUFFER_QUEUE_IS_EMPTY );
	
	assert_ptr_equal( q.tail, q.head );
}


void
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


void
test_push_overwrite( void **state )
{
	(void) state;

	buffer_queue_t q;
	uint8_t buf[ 12 ];
	
	slave_buffer_queue_init( &q, buf, sizeof( buf ), BUFFER_QUEUE_OVERWRITE );

	// вставка первого элемента, размером меньше самой очереди -------------
	
	uint32_t a[] = { 0xFFEEDD, 0xCCBBAA };
	
	slave_buffer_queue_push( &q, &a, sizeof( a ) );
	assert_memory_equal( a, buf, sizeof( a ) );
	
	assert_uint_equal( q.flags & BUFFER_QUEUE_IS_EMPTY, 0 );

	// вставка второго элемента, размером больше оставшегося места в очереди
	// (т.е. то место, где должна быть перезапись)

	uint32_t b[] = { 0xAABCCBAA, 0xDDEFFEDD };
	
	bool status = slave_buffer_queue_push( &q, &b, sizeof( b ) );
	
	assert_memory_equal( b, buf + sizeof( a ), sizeof( b[ 0 ] ) );
	assert_memory_equal( b + sizeof( b[ 0 ] ), buf, sizeof( b[ 1 ] ) );
	
	assert_true( status );

	assert_uint_equal( q.flags & BUFFER_QUEUE_IS_FULL, BUFFER_QUEUE_IS_FULL );

	assert_ptr_equal( buf + sizeof( b[ 0 ] ), q.tail );
	assert_ptr_equal( q.head, q.tail );

	// перезапись всей очереди --------------------------------------------

	uint8_t c[ sizeof( buf )*2 + sizeof( buf )/2 ];

	uint16_t cx = 0x0201;
	int i = sizeof( c ) - sizeof( buf );
	
	for ( ; i < sizeof( c ); i += 2, cx += 0x0202 ) *( (uint16_t *) &c[ i ] ) = cx;

	status = slave_buffer_queue_push( &q, &c, sizeof( c ) );

	uint8_t *c_start = &( c[ sizeof( c ) - sizeof( buf ) ] );
	
	assert_memory_equal( c_start,                    q.tail,  q.end - q.head );
	assert_memory_equal( c_start + (q.end - q.tail), q.start, q.tail - q.start );
	
	assert_true( status );

	assert_ptr_equal( q.head, q.tail );

	assert_uint_equal( q.flags & BUFFER_QUEUE_IS_FULL, BUFFER_QUEUE_IS_FULL );
}
