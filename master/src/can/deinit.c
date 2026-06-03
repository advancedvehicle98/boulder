#include <master/can.h>

#include <sys/socket.h>
#include <unistd.h>


void
master_can_deinit( __STATE can_state_t *s )
{
	if ( ! s || ! s->ready ) return;

	close( s->socket_fd );

	s->ready = false;
}
