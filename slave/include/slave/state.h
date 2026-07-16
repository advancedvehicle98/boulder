#ifndef __BOULDER_SLAVE_STATE_H
#define __BOULDER_SLAVE_STATE_H


#include <common/defines.h>
#include <slave/ahrs.h>
#include <slave/can.h>
#include <slave/motors.h>
#include <slave/range.h>
#include <slave/uart.h>
#include <soc/state.h>


typedef enum {
	SLAVE_RUN_CONTINUE,
	SLAVE_RUN_SHUTDOWN_REQUEST_FROM_MASTER
} boulder_run_status_t;


typedef enum {
	SLAVE_INIT_SUCCESS,
	SLAVE_INIT_FAILED_CAN_INIT,
	SLAVE_INIT_FAILED_UART_INIT,
	SLAVE_INIT_FAILED_MOTORS_INIT
} boulder_init_error_t;


typedef struct _boulder_slave_state_t {
	soc_state_t    soc;
	can_state_t    can;
	uart_state_t   uart;
	motors_state_t motors;
	ahrs_state_t   ahrs;
	range_state_t  range;
} boulder_slave_state_t;


// это для обработчиков. значение копируется из переменной в main
extern boulder_slave_state_t *boulder;


// init.c
__COLD boulder_init_error_t slave_init( __STATE boulder_slave_state_t *s );
__COLD __NORETURN void slave_init_log_and_halt( __IN const boulder_init_error_t e );
// run.c
__HOT boulder_run_status_t slave_run( __STATE boulder_slave_state_t *s );
// handle_exit.c
__COLD __NORETURN void slave_handle_exit( __STATE boulder_slave_state_t *s, __IN const boulder_run_status_t e );


#endif // ! __BOULDER_SLAVE_STATE_H
