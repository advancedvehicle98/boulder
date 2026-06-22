#ifndef __BOUDLER_SOC_H
#define __BOUDLER_SOC_H


#include <soc/state.h>

#include <common/defines.h>


typedef enum {
	SOC_INIT_SUCCESS,
	SOC_INIT_FAILED_ARCH_INIT
} soc_init_error_t;


// socs/<soc>/init.c
soc_init_error_t slave_soc_init( __STATE soc_state_t *soc );


#endif // ! __BOUDLER_SOC_H
