#include <master/debug.h>
#include <master/state.h>

#include <common/defines.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void _print_help( void );


int
main( const int argc,
	  const char *argv[] )
{
	boulder_state_t state;
	boulder_state_args_t state_args;
	
	can_state_t can_state;

	// парсим аргументы, если есть
	
	if ( argc > 1 ) {
		// помосчь
		if ( argc == 2
			 && (    _str_equal( argv[ 1 ], "-h" )
				  || _str_equal( argv[ 1 ], "--h" ) ) ) {
			_print_help();
			return EXIT_SUCCESS;
		}
		
		uint32_t parse_status = master_parse_args( &state_args, argc-1, &( argv[1] ) );

		if ( parse_status != EXIT_SUCCESS ) {
			_print_help();
			return EXIT_FAILURE;
		}
	}

	// инициализация всего нужного говна

	uint32_t init_status = master_init( &state, &state_args );

	if ( init_status != EXIT_SUCCESS ) {
		puts( "Не удалось запустить boulder" );
		return EXIT_FAILURE;
	}

	boulder_run_status_t run_status;

	do {
		run_status = master_run( &state );
	} while ( run_status == MASTER_RUN_CONTINUE );
	
	master_deinit( &state );

	if ( run_status != MASTER_RUN_FINISH )
		return EXIT_FAILURE;
	
	return EXIT_SUCCESS;
}


void
_print_help( void )
{
	puts( "\nОпции: "
		  "\n\t" );
}
