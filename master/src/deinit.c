#include <master/state.h>


void
master_deinit( __STATE boulder_state_t *s )
{
	master_can_deinit( &( s->can ) );
}
