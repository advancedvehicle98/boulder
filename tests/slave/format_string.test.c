#include <slave/ram.h>

#include <cmocka.h>
#include <stdarg.h>


static void test_no_formatting( void* *state );
static void test_overflow( void * *state );
static void test_put_chars( void * *state );
static void test_put_string( void * *state );
static void test_put_integers( void * *state );


int
main( void )
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test( test_no_formatting ),
		cmocka_unit_test( test_put_chars ),
		cmocka_unit_test( test_put_string ),
		cmocka_unit_test( test_put_integers ),
		cmocka_unit_test( test_overflow )
	};

	return cmocka_run_group_tests( tests, NULL, NULL );
}


void
test_no_formatting( void* *state )
{
	(void) state;
	
	char buf[ 16 ];
	const char *fmt = "example";

	size_t size = slave_ram_format_string( buf, 16, fmt );

	assert_uint_equal( size, 8 );
	assert_memory_equal( buf, fmt, 8 );
}


void
test_overflow( void * *state )
{
	(void) state;
	
	char buf[ 6 ];

	size_t size = slave_ram_format_string( buf, 6, "%u4", 0xBBBBBBBB );

	assert_uint_equal( size, 6 );
	assert_memory_equal( buf, "0xBBBB", 6 );
}


void
test_put_chars( void * *state )
{
	(void) state;

	char buf[ 4 ];
	const char *str = "abc";

	size_t size = slave_ram_format_string( buf, 4, "%c%c%c", 'a', 'b', 'c' );

	assert_uint_equal( size, 4 );
	assert_memory_equal( buf, str, 4 );
}


void
test_put_string( void * *state )
{
	(void) state;

	char buf[ 16 ];
	const char *str = "Hello world!!!";

	size_t size = slave_ram_format_string( buf, 16, "Hello %s%s", "world", "!!!" );

	assert_uint_equal( size, 15 );
	assert_memory_equal( buf, str, 15 );
}


void
test_put_integers( void * *state )
{
	(void) state;

	char buf[ 32 ];
	const char *str = "12 -5 0xAB 0b10010 0xCAFEBABE";

	size_t size = slave_ram_format_string(
		buf, 32, "%u1 %i1 %x1 %b1 %x4", 12, -5, 0xAB, 0b10010, 0xCAFEBABE );

	assert_uint_equal( size, 30 );
	assert_memory_equal( buf, str, 30 );
}
