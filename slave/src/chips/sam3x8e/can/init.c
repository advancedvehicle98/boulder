#include <slave/can.h>

#include <slave/chips/sam3x8e.h>

// --------------------------------------------------
#include <common/config.h>
// --------------------------------------------------


can_init_error_t
slave_chip_can_init( __STATE can_chip_specific_state_t *s )
{
	// !!! тактовый сигнал для CAN активируется сразу же
	//     при настройке камня
	// ( см. slave_chip_init и _enable_peripheral_clocks )

	
	
#ifdef CONFIG_SLAVE_CAN_IFACE_CAN0
	s->iface = sam3x8e_can0;
#else
	s->iface = sam3x8e_can1;
#endif
	
	return CAN_INIT_SUCCESS;
}
