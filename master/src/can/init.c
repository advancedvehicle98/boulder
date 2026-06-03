#include <master/can.h>

#include <linux/can/raw.h>
#include <sys/socket.h>

#include <stdlib.h>
#include <string.h>


can_init_error_t
master_can_init( __STATE       can_state_t      *s,
				 __IN    const can_state_args_t *args )
{
	if ( s->ready ) return CAN_INIT_SUCCESS;

	uint32_t fd = socket( PF_CAN, SOCK_RAW, CAN_RAW );

	if ( fd < 0 ) return CAN_INIT_ERROR_NO_SOCKET;

	if ( bind(  ) ) return CAN_INIT_ERROR_CANNOT_BIND;
	
	const char *if_name = args->if_name
		                ? args->if_name
		                : master_can_default_if_name;
	
	// присвание говна идёт строго в конце после всего остального
	s->socket_fd = fd;
	
	strcpy_s( s->if_name, if_name, MASTER_CAN_IF_NAME_LEN );
	
	s->ready = true;
	
	return CAN_INIT_SUCCESS;
}
