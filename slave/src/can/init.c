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

	// окончательно инициализируем структуру с состоянием -------------------
	
	s->ready = true;
	
	return CAN_INIT_SUCCESS;
}
