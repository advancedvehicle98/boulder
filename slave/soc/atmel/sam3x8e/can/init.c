#include <slave/can.h>


// см. официальные доки libsam
// в частности:
// https://github.com/arduino/ArduinoCore-sam/blob/790ff2c852bf159787a9966bddee4d9f55352d15/system/libsam/include/can.h

can_init_error_t
slave_soc_can_init( __STATE can_soc_state_t *s )
{
	// Настройка значений в соответствии с конфигом -------------
	
#ifdef CONFIG_SAM3X8E_USE_CAN1
	Can * const    can    = CAN1;
	const uint32_t can_id = ID_CAN1;
#else
	Can * const    can    = CAN0;
	const uint32_t can_id = ID_CAN0;
#endif

	// Включаем тактирование для CAN-интерфейса -----------------

	pmc_enable_periph_clk( can_id );

	// инициализируем интерфейс

	/* can_init( can,  ); */
	
	// присваивание данных в состояние --------------------------
	
	s->can       = can;
	s->periph_id = can_id;
	
	return CAN_INIT_SUCCESS;
}
