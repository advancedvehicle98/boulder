#ifndef __BOULDER_MASTER_CAN_H
#define __BOULDER_MASTER_CAN_H


#include <common/defines.h>

#include <net/if.h>
#include <stdint.h>
#include <sys/ioctl.h>


#define MASTER_CAN_IF_NAME_LEN IFNAMSIZ


typedef enum {
	CAN_INIT_SUCCESS,
	CAN_INIT_ERROR_NO_SOCKET,
	CAN_INIT_ERROR_CANNOT_BIND
} can_init_error_t;


typedef struct _can_state_t {
	char if_name[ MASTER_CAN_IF_NAME_LEN ];
	bool ready;

	uint32_t socket_fd;
} can_state_t;


typedef struct _can_state_args_t {
	char *if_name;
	
} can_state_args_t;


extern const char master_can_default_if_name[ MASTER_CAN_IF_NAME_LEN ];


// can/deinit.c
void master_can_deinit( __STATE can_state_t *s );
// can/init.c
can_init_error_t master_can_init( __STATE can_state_t *s, __IN const can_state_args_t *args );


#endif // ! __MASTER_CAN_H
