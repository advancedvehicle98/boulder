#ifndef __BOUDLER_BOARD_H
#define __BOUDLER_BOARD_H


#include <slave/soc.h>

#include <common/defines.h>


typedef enum {
	BOARD_INIT_SUCCESS,
	BOARD_INIT_FAILED_SOC_INIT
} board_init_error_t;


// boards/<board>/init.c
board_init_error_t slave_board_init( __STATE soc_state_t *soc );
__NORETURN void slave_board_init_log_and_halt( __IN const board_init_error_t e );


#endif // ! __BOUDLER_BOARD_H
