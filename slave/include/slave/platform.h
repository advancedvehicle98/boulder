#ifndef __BOULDER_SLAVE_PLATFORM_H
#define __BOULDER_SLAVE_PLATFORM_H


#include <common/defines.h>


typedef enum {
	PLATFORM_INIT_SUCCESS,
	PLATFORM_FAILED_CHIP_INIT
} platform_init_status_t;


// chips/$(CONFIG_SLAVE_PLATFORM)/init.c
__COLD platform_init_status_t slave_platform_init( void );
__COLD __NORETURN void slave_platform_init_log_and_halt( __IN const platform_init_status_t e );


#endif // ! __BOULDER_SLAVE_PLATFORM_H
