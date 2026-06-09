#include <master/state.h>

#include <common/defines.h>

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


size_t _interpret_cmd_arg( __STATE boulder_state_args_t *args,
						   __IN const char *opt,
						   __IN const char *tail[] );

size_t _try_as_can_bitrate( __STATE boulder_state_args_t *args,
							__IN const char *opt,
							__IN const char *tail[] );

size_t _try_as_can_iface_name( __STATE boulder_state_args_t *args,
							   __IN const char *opt,
							   __IN const char *tail[] );

typedef size_t ( *try_as_func_t )( boulder_state_args_t *, const char *, const char *[] );
	
try_as_func_t _try_as[] = {
	_try_as_can_bitrate,
	_try_as_can_iface_name
};

#define TRY_AS_COUNT sizeof( _try_as )/sizeof( try_as_func_t )


uint32_t
master_parse_args( __STATE        boulder_state_args_t *args_out,
				   __IN     const size_t                argc,
				   __IN     const char                 *argv[] )
{
	int di = 0;

	// проходимся по всем возможным аргументам
	for ( int i = 0; i < argc; i += di ) {
		di = _interpret_cmd_arg( args_out, argv[ i ], &( argv[ i+1 ] ) );
		if ( ! di ) return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}


// возвращает кол-во интерпретированных аргументов командной строки
size_t
_interpret_cmd_arg( __STATE       boulder_state_args_t *args_out,
					__IN    const char                 *opt,
					__IN    const char                 *tail[] )
{
	for ( size_t a = 0; a < TRY_AS_COUNT; ++a ) {
		size_t offset = _try_as[ a ]( args_out, opt, tail );
		
		if ( offset ) return offset;
	}
													
	return 0;
}


size_t
_try_as_can_bitrate( __STATE       boulder_state_args_t *args,
					 __IN    const char                 *opt,
					 __IN    const char                 *tail[] )
{
	if ( ! _str_equal( opt, "--can-bitrate" ) ) return 0;

	if ( ! tail[ 0 ] ) return 0;

	uint32_t bitrate = atoi( tail[ 0 ] );

	const uint32_t allowed_values[] = _allowed_can_bitrate_values;
	const size_t allowed_values_count = sizeof( allowed_values )
		                              / sizeof( uint32_t );
	
	int i = 0;

	for ( ; i < allowed_values_count; ++i )
		if ( bitrate == allowed_values[ i ] ) break;

	if ( i == allowed_values_count ) return 0;
	
	can_state_args_t *can = &( args->can );	
	can->bitrate = bitrate;
	
	return 2;
}


size_t
_try_as_can_iface_name( __STATE       boulder_state_args_t *args_out,
						__IN    const char                 *opt,
						__IN    const char                 *tail[] )
{	
	if ( ! _str_equal( opt, "--can-iface-name" ) ) return 0;

	if ( ! tail[ 0 ] || strlen( tail[ 0 ] ) > MASTER_CAN_IF_NAME_LEN-1 ) return 0;

	can_state_args_t *can = &( args_out->can );	
	can->if_name = tail[ 0 ];

	return 2;
}
