#include <slave/can.h>

#include <common/config.h>
#include <slave/state.h>

#include <stddef.h>


// см. официальные доки libsam
// в частности:
// https://github.com/arduino/ArduinoCore-sam/blob/790ff2c852bf159787a9966bddee4d9f55352d15/system/libsam/include/can.h

can_init_error_t
slave_soc_can_init( __STATE can_soc_state_t *s )
{
	// !!! тактирование CAN-шины через PMC и
	//     GPIO-функции были включены в slave_soc_init

	// инициализируем интерфейс ---------------------------------

	// значения битрейта соответствуют макросам CAN_BPS_* из libsam
	uint32_t init_status = can_init( CAN_IF,
									 CONFIG_SLAVE_SOC_CLOCK_FREQUENCY,
									 CONFIG_CAN_BITRATE_KBPS );

	// см. soc/<soc>/init.c, т.к. кажется, что неправильно
	// замеряется mclk

	if ( ! init_status ) return CAN_INIT_TIMEOUT;

	// сброс почтовых ящиков -------------------------------
	
	int mbox_idx = 0;

	// для трансмиттера
	
	for ( ; mbox_idx < CONFIG_SAM3X8E_CAN_TX_MBOX_COUNT; ++mbox_idx ) {
		can_mb_conf_t *conf = &s->mbox_conf[ mbox_idx ];
		
		conf->ul_mb_idx   = mbox_idx;
		conf->ul_id       = CAN_MID_MIDvB( CONFIG_DRONECAN_SLAVE_ID ); 	
		conf->uc_obj_type = CAN_MB_TX_MODE;
		conf->uc_id_ver   = 1; // с использоавнием расширенного ID
	
		can_mailbox_init( CAN_IF, conf );
	}
	
	// для ресивера

	for ( ; mbox_idx < SAM3X8E_CAN_MBOX_COUNT; ++mbox_idx ) {
		can_mb_conf_t *conf = &s->mbox_conf[ mbox_idx ];
		
		conf->ul_mb_idx   = mbox_idx;
		conf->ul_id       = CAN_MID_MIDvB( CONFIG_DRONECAN_SLAVE_ID ); 		
		conf->uc_obj_type = CAN_MB_RX_MODE;
		conf->ul_id_msk   = CAN_MAM_MIDvA_Msk | CAN_MAM_MIDvB_Msk;
		conf->uc_id_ver   = 1;

		can_mailbox_init( CAN_IF, conf );
	}

	// включение прерываний от шины ------------------------

	// цитируя автора due_can (collin@github.com):

	// set a fairly low priority so almost anything can preempt.
	// this has the effect that most anything can interrupt our interrupt handler
	// that's a good thing because the interrupt handler is long and complicated
	// and can send callbacks into user code which could also be long and complicated.
	// But, keep in mind that user code in callbacks runs in interrupt context
	// but can still be preempted at any time.

	NVIC_SetPriority( CAN_IRQ, 12 );
	NVIC_EnableIRQ( CAN_IRQ );
	
	return CAN_INIT_SUCCESS;
}
