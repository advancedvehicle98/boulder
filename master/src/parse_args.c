#include <master/state.h>

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


size_t _interpret_cmd_arg( __STATE boulder_state_args_t *args,
						   __IN const char *opt,
						   __IN const char *tail[] );

size_t _try_as_can_iface_name( __STATE boulder_state_args_t *args,
							   __IN const char *opt,
							   __IN const char *tail[] );

typedef size_t ( *try_as_func_t )( boulder_state_args_t *, const char *, const char * );
	
try_as_func_t _try_as[] = {
	_try_as_can_iface_name
};

#define TRY_AS_COUNT sizeof( _try_as )/sizeof( try_as_func_t )


uint32_t
master_parse_args( __STATE        boulder_state_args_t *args_out,
				   __IN     const size_t                argc,
				   __IN     const char                 *argv[] )
{
	int di = 0;
	
	for ( int i = 0; i < argc; i += di ) {
		di = _interpret_cmd_arg( args_out, argv[ i ], &( argv[ i+1 ] ) );
		if ( ! di ) return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}


size_t
_interpret_cmd_arg( __STATE       boulder_state_args_t *args_out,
					__IN    const char                 *opt,
					__IN    const char                 *tail[] )
{
	for ( size_t a = 0; a < TRY_AS_COUNT; ++a ) {
		size_t offset = _try_as[ a ]( opt, tail );
		
		if ( offset ) return offset;
	}
													
	return 0;
}


size_t
_try_as_can_iface_name( __STATE       boulder_state_args_t *args_out,
						__IN    const char                 *opt,
						__IN    const char                 *tail[] )
{
	if ( ! _str_equal( opt, "--can-iface-name" ) ) return 0;

	if ( ! tail[ 0 ] || strlen( tail[ 0 ] ) > MASTER_CAN_IF_NAME_LEN ) return 0;
	
	can_state_args_t *can = &( args_out->can );
	can->if_name = tail[ 0 ];

	if ( strncmp( tail[ 0 ], "can", 3 ) == 0 )
		can->if_index = atoi( &( tail[ 0 ][ 3 ] ) ); // если че то не то, то один хуй индекс нулевой
	else ( strncmp( tail[ 0 ], "vcan", 4 ) == 0 )
		can->if_index = atoi( &( tail[ 0 ][ 4 ] ) );

	return 2;
}
