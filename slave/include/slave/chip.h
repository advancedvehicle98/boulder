#ifndef __BOULDER_SLAVE_CHIP_H
#define __BOULDER_SLAVE_CHIP_H


#include <common/defines.h>


typedef enum {
	CHIP_INIT_SUCCESS
} chip_init_status_t;


typedef void ( *handler_func_t )( void );


__NORETURN void main( void );
void __libc_init_array( void );


// chips/$(CONFIG_SLAVE_CHIP)/general_purpose_handler.c
void slave_general_purpose_handler( void );
// chips/$(CONFIG_SLAVE_CHIP)/init.c
__COLD chip_init_status_t slave_chip_init( void );
__COLD void slave_chip_init_log( __IN const chip_init_status_t e );
// chips/$(CONFIG_SLAVE_CHIP)/reset.c
void slave_chip_reset( void );


#endif // ! __BOULDER_SLAVE_CHIP_H
