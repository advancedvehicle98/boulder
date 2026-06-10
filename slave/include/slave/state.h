#ifndef __BOULDER_SLAVE_STATE_H
#define __BOULDER_SLAVE_STATE_H


#include <slave/can.h>

#include <soc/state.h>

#include <common/defines.h>


typedef enum {
	SLAVE_RUN_CONTINUE,
} boulder_run_status_t;


typedef enum {
	SLAVE_INIT_SUCCESS,
} boulder_init_error_t;


typedef struct _boulder_slave_state_t {
	soc_state_t soc;
	can_state_t can;
} boulder_slave_state_t;


// init.c
__COLD boulder_init_error_t slave_init( __STATE boulder_slave_state_t *s );
__COLD __NORETURN void slave_init_log_and_halt( __IN const boulder_init_error_t e );
// run.c
__HOT boulder_run_status_t slave_run( __STATE boulder_slave_state_t *s );
// handle_exit.c
__COLD __NORETURN void slave_handle_exit( __STATE boulder_slave_state_t *s, __IN const boulder_run_status_t e );


#endif // ! __BOULDER_SLAVE_STATE_H
