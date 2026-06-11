#include <slave/can.h>

#include <common/config.h>

#include <stddef.h>


static void _enable_interrupt( void );
static void _reset_mbox( __STATE can_soc_state_t * const can );


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

	// !!! тактирование CAN-шины через PMC и
	//     GPIO-функции были включены в slave_soc_init

	// инициализируем интерфейс ---------------------------------

	// значения битрейта соответствуют макросам CAN_BPS_* из libsam
	uint32_t init_status = can_init( can, CHIP_FREQ_CPU_MAX, CONFIG_CAN_BITRATE_KBPS );

	if ( ! init_status ) return CAN_INIT_TIMEOUT;
	
	// присваивание данных в состояние --------------------------
	
	s->iface     = can;
	s->periph_id = can_id;

	// сброс почтовых ящиков -------------------------------

	_reset_mbox( s );

	// включение прерываний от шины ------------------------

	_enable_interrupt();
	
	return CAN_INIT_SUCCESS;
}


void
_enable_interrupt()
{
#ifdef CONFIG_SAM3X8E_USE_CAN1
	const uint32_t irq = CAN1_IRQn;
#else
	const uint32_t irq = CAN0_IRQn;
#endif

	// цитируя автора due_can (collin@github.com):

	// set a fairly low priority so almost anything can preempt.
	// this has the effect that most anything can interrupt our interrupt handler
	// that's a good thing because the interrupt handler is long and complicated
	// and can send callbacks into user code which could also be long and complicated.
	// But, keep in mind that user code in callbacks runs in interrupt context
	// but can still be preempted at any time.

	NVIC_SetPriority( irq, 12 );
	NVIC_EnableIRQ( irq );
}


void
_reset_mbox( __STATE can_soc_state_t * const can )
{
	// считаем кол-во ящиков для трансмиттера
	
	size_t tx_mbox_count = CONFIG_SAM3X8E_CAN_TX_MBOX_COUNT;

	if      ( tx_mbox_count < CONFIG_SAM3X8E_CAN_TX_MBOX_COUNT_MIN )
		tx_mbox_count = CONFIG_SAM3X8E_CAN_TX_MBOX_COUNT_MIN;
	else if ( tx_mbox_count > CONFIG_SAM3X8E_CAN_TX_MBOX_COUNT_MAX )
		tx_mbox_count = CONFIG_SAM3X8E_CAN_TX_MBOX_COUNT_MAX;

	can->tx_mbox_count = tx_mbox_count;
	
	// сброс ящиков до исходного состояния
	
	int mbox_idx = 0;

	// для трансмиттера
	
	for ( ; mbox_idx < tx_mbox_count; ++mbox_idx ) {
		can_mb_conf_t *conf = &( can->mbox_conf[ mbox_idx ] );
		
		conf->ul_mb_idx   = mbox_idx;
		conf->ul_id       = CAN_MID_MIDvB( CONFIG_DRONECAN_SLAVE_ID ); 	
		conf->uc_obj_type = CAN_MB_TX_MODE;
		conf->uc_id_ver   = 1; // с использоавнием расширенного ID
	
		can_mailbox_init( can->iface, conf );
	}
	
	// для ресивера

	for ( ; mbox_idx < SAM3X8E_CAN_MBOX_COUNT; ++mbox_idx ) {
		can_mb_conf_t *conf = &( can->mbox_conf[ mbox_idx ] );
		
		conf->ul_mb_idx   = mbox_idx;
		conf->ul_id       = CAN_MID_MIDvB( CONFIG_DRONECAN_SLAVE_ID ); 		
		conf->uc_obj_type = CAN_MB_RX_MODE;
		conf->ul_id_msk   = CAN_MAM_MIDvA_Msk | CAN_MAM_MIDvB_Msk;
		conf->uc_id_ver   = 1; // с использоавнием расширенного ID

		can_mailbox_init( can->iface, conf );
	}
}
