#include <slave/can.h>

#include <stdbool.h>


can_init_error_t
slave_can_init( __STATE can_state_t *s )
{
	s->ready = false;

	// делаем специфичную для микроконтроллера настройку ---------------------------------
	
	can_soc_state_t *soc = &( s->soc );
	can_init_error_t init_state;

	init_state = slave_soc_can_init( soc );

	if ( init_state != CAN_INIT_SUCCESS ) return init_state;

	// настройка canard --------------------------------------------------------

	canardInit( &( s->canard ),
				s->canard_pool, CONFIG_CANARD_SLAVE_POOL_SIZE,
				slave_can_on_receive,
				slave_can_should_accept_transfer,
				s );

	canardSetLocalNodeID( &( s->canard ), CONFIG_DRONECAN_SLAVE_ID );

	// окончательно инициализируем структуру с состоянием -------------------
	
	s->ready = true;
	
	return CAN_INIT_SUCCESS;
}
