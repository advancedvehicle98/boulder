#ifndef __BOULDER_MASTER_STATE_H
#define __BOULDER_MASTER_STATE_H

#include <master/can.h>

#include <common/defines.h>

#include <stddef.h>
#include <stdint.h>


typedef enum {
	MASTER_RUN_FINISH,
	MASTER_RUN_CONTINUE
} boulder_run_status_t;


typedef struct _boulder_state_t {
	can_state_t can;
} boulder_state_t;


typedef struct _boulder_state_args_t {
	can_state_args_t can;
} boulder_state_args_t;


// deinit.c
void master_deinit( __STATE boulder_state_t *s );
// init.c
uint32_t master_init( __STATE boulder_state_t *s, __IN const boulder_state_args_t *args );
// parse_args.c
uint32_t master_parse_args( __STATE boulder_state_args_t *args, __IN const size_t argc, __IN const char *argv[] );
// run.c
boulder_run_status_t master_run( __STATE boulder_state_t *s );


#endif // ! __BOULDER_MASTER_STATE_H
