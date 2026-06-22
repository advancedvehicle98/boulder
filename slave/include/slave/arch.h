#ifndef __BOULDER_SLAVE_ARCH_H
#define __BOULDER_SLAVE_ARCH_H


#include <arch/custom_errors.h>


typedef enum {
	ARCH_INIT_SUCCESS,
#ifdef ARCH_INIT_CUSTOM_ERRORS
	ARCH_INIT_CUSTOM_ERRORS // arch/custom_errors.h
#endif
} arch_init_error_t;


// arch/<arch>/init.c
arch_init_error_t slave_arch_init( void );


#endif // ! __BOULDER_SLAVE_ARCH_H
