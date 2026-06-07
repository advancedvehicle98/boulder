#ifndef __BOULDER_SLAVE_PLATFORM_H
#define __BOULDER_SLAVE_PLATFORM_H

#include <common/defines.h>


typedef enum {
	PLATFORM_INIT_SUCCESS,
	PLATFORM_FAILED_CHIP_INIT
} platform_init_error_t;


// chips/$(CONFIG_SLAVE_PLATFORM)/init.c
__COLD platform_init_error_t slave_platform_init( void );
__COLD __NORETURN void slave_platform_init_log_and_halt( __IN const platform_init_error_t e );


#ifdef CONFIG_SLAVE_PLATFORM_studica_blackhawk
#   include <slave/platforms/studica_blackhawk.h>
#endif


#endif // ! __BOULDER_SLAVE_PLATFORM_H
