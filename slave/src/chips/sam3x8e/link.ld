/*
	см. https://github.com/bobc/bare-metal-sam3x/blob/master/blink/flash.ld
*/

OUTPUT_FORMAT( "elf32-littlearm", "elf32-littlearm", "elf32-littlearm" )
OUTPUT_ARCH( arm )


MEMORY {
	rom   ( rx )  : ORIGIN = 0x00080000, LENGTH = 0x00080000
	sram0 ( rwx ) : ORIGIN = 0x20000000, LENGTH = 0x00010000
	sram1 ( rwx ) : ORIGIN = 0x20080000, LENGTH = 0x00008000
	ram   ( rwx ) : ORIGIN = 0x20070000, LENGTH = 0x00018000
}


SECTIONS {
	.text : {
		. = ALIGN( 4 );
		_sfixed = .;
		KEEP( *( .vectors .vectors.* ) )
		*( .text .text.* .gnu.linkonce.t.* )
		*( .glue_7t ) *( .glue_7 )
		*( .ARM.extab* .gnu.linkonce.armextab.* )

		. = ALIGN( 4 );
		KEEP( *( .init ) )
		. = ALIGN( 4 );
		__preinit_array_start = .;
		KEEP( *( .preinit_array ) )
		__preinit_array_end = .;

		. = ALIGN( 4 );
		__init_array_start = .;
		KEEP( *( SORT( .init_array.* ) ) )
		KEEP( *( .init_array ) )
		__init_array_end = .;

		. = ALIGN( 4 );
		KEEP( *crtbegin.o( .ctors ) )
		KEEP( *( EXCLUDE_FILE( *crtend.o ) .ctors ) )
		KEEP( *( SORT( .ctors.* ) ) )
		KEEP( *crtend.o( .ctors ) )

		. = ALIGN( 4 );
		KEEP( *( .fini ) )

		. = ALIGN( 4 );
		__fini_array_start = .;
		KEEP( *( .fini_array ) )
		KEEP( *( SORT( .fini_array.* ) ) )
		__fini_array_end = .;

		KEEP( *crtbegin.o( .dtors ) )
		KEEP( *( EXCLUDE_FILE( *crtend.o ) .dtors ) )
		KEEP( *( SORT( .dtors.* ) ) )
		KEEP( *crtend.o( .dtors ) )

		. = ALIGN( 4 );
		_efixed = .;
	} > rom /* .text */

	PROVIDE_HIDDEN ( __exidx_start = . );
	.ARM.exidx : {
				*( .ARM.exidx* .gnu.linkonce.armexidx.* )
	} > rom
	PROVIDE_HIDDEN ( __exidx_end = . );

	. = ALIGN( 4 );
	_etext = .;

	.relocate : AT ( _etext ) {
		. = ALIGN( 4 );
		_srelocate = .;
		*( .ramfunc .ramfunc.* );
		*( .data .data.* );
		. = ALIGN( 4 );
		_erelocate = .;
	} > ram

	.bss ALIGN( 4 ) ( NOLOAD ) : {
		. = ALIGN( 4 );
		_sbss = .;
		_szero = .;
		*( .bss .bss.* )
		*( COMMON )
		. = ALIGN( 4 );
		_ebss = .;
		_ezero = .;
	} > ram

	. = ALIGN( 4 );
	_end = .;

	.stack_dummy : {
		*( .stack* )
	} > ram

	_stack_top = ORIGIN( ram ) + LENGTH( ram );
	_stack_limit = _stack_top - SIZEOF( .stack_dummy );

	PROVIDE( _sstack = _stack_limit );
	PROVIDE( _estack = _stack_top );
}
