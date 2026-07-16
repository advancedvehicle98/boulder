#include <slave/can.h>

#include <common/config.h>
#include <slave/state.h>
#include <soc/can.h>

#include <stddef.h>


// от atmel (там эта функция static)
static uint32_t can_set_baudrate( void );


// см. официальные доки libsam
// в частности:
// https://github.com/arduino/ArduinoCore-sam/blob/790ff2c852bf159787a9966bddee4d9f55352d15/system/libsam/include/can.h

can_init_error_t
slave_soc_can_init( __STATE can_soc_state_t *s )
{
	// !!! тактирование CAN-шины через PMC и
	//     GPIO-функции были включены в slave_soc_init

	uint32_t status;
	
	CAN_IF->CAN_MR &= ~CAN_MR_CANEN;

	// настройка бода ---------------------------------

	// значения битрейта соответствуют макросам CAN_BPS_* из libsam
	status = can_set_baudrate();

	if ( ! status ) return CAN_INIT_ERROR_FAILED_BAUDRATE_SET;

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

	// включение контроллера -----------------------------------

	CAN_IF->CAN_MR |= CAN_MR_CANEN;

	// ждём, пока CAN не синхронизируется с шиной --------------

	uint32_t ticks = 0;
	
	do {
		status = CAN_IF->CAN_SR;
		if ( ticks++ > CONFIG_SAM3X8E_CAN_TIMEOUT_TICKS )
			return CAN_INIT_ERROR_TIMEOUT;
	} while ( ! ( status & CAN_SR_WAKEUP ) );
	
	return CAN_INIT_SUCCESS;
}

	
uint32_t
can_set_baudrate( void )
{
	//  uc_tq uc_prog    uc_phase1  uc_phase2  uc_sjw     uc_sp
	const can_bit_timing_t bit_time[] = {
		{ 8,  ( 2 + 1 ), ( 1 + 1 ), ( 1 + 1 ), ( 2 + 1 ), 75 },
		{ 9,  ( 1 + 1 ), ( 2 + 1 ), ( 2 + 1 ), ( 1 + 1 ), 67 },
		{ 10, ( 2 + 1 ), ( 2 + 1 ), ( 2 + 1 ), ( 2 + 1 ), 70 },
		{ 11, ( 3 + 1 ), ( 2 + 1 ), ( 2 + 1 ), ( 3 + 1 ), 72 },
		{ 12, ( 2 + 1 ), ( 3 + 1 ), ( 3 + 1 ), ( 3 + 1 ), 67 },
		{ 13, ( 3 + 1 ), ( 3 + 1 ), ( 3 + 1 ), ( 3 + 1 ), 77 },
		{ 14, ( 3 + 1 ), ( 3 + 1 ), ( 4 + 1 ), ( 3 + 1 ), 64 },
		{ 15, ( 3 + 1 ), ( 4 + 1 ), ( 4 + 1 ), ( 3 + 1 ), 67 },
		{ 16, ( 4 + 1 ), ( 4 + 1 ), ( 4 + 1 ), ( 3 + 1 ), 69 },
		{ 17, ( 5 + 1 ), ( 4 + 1 ), ( 4 + 1 ), ( 3 + 1 ), 71 },
		{ 18, ( 4 + 1 ), ( 5 + 1 ), ( 5 + 1 ), ( 3 + 1 ), 67 },
		{ 19, ( 5 + 1 ), ( 5 + 1 ), ( 5 + 1 ), ( 3 + 1 ), 68 },
		{ 20, ( 6 + 1 ), ( 5 + 1 ), ( 5 + 1 ), ( 3 + 1 ), 70 },
		{ 21, ( 7 + 1 ), ( 5 + 1 ), ( 5 + 1 ), ( 3 + 1 ), 71 },
		{ 22, ( 6 + 1 ), ( 6 + 1 ), ( 6 + 1 ), ( 3 + 1 ), 68 },
		{ 23, ( 7 + 1 ), ( 7 + 1 ), ( 6 + 1 ), ( 3 + 1 ), 70 },
		{ 24, ( 6 + 1 ), ( 7 + 1 ), ( 7 + 1 ), ( 3 + 1 ), 67 },
		{ 25, ( 7 + 1 ), ( 7 + 1 ), ( 7 + 1 ), ( 3 + 1 ), 68 }
	};
	
	uint8_t tq, prescale;
	uint32_t mod, current_mod, div;

	tq = SAM3X8E_CAN_MIN_TQ_NUM;
	mod = 0xffffffff;
	
	// ищем нужный квант для SAM3X8E_CAN_BAUD_MAX_DIV ---------------------
	
	for ( uint8_t i = SAM3X8E_CAN_MIN_TQ_NUM; i <= SAM3X8E_CAN_MAX_TQ_NUM; ++i ) {
		div = CONFIG_CAN_BITRATE_KBPS * i * 1000;
		prescale = CONFIG_SLAVE_SOC_CLOCK_FREQUENCY / div;
		
		if ( prescale > SAM3X8E_CAN_BAUD_MAX_DIV ) continue;
		
		current_mod = CONFIG_SLAVE_SOC_CLOCK_FREQUENCY % div;
		
		if ( current_mod >= mod ) continue;
		
		mod = current_mod;
		tq = i;

		if ( ! mod ) break;
	}

	can_bit_timing_t *bt = (can_bit_timing_t *) &bit_time[ tq - SAM3X8E_CAN_MIN_TQ_NUM ];

	// запись в регистр ----------------------------------------------------
	
	CAN_IF->CAN_BR = CAN_BR_PHASE2( bt->phase2 - 1 )
		  		   | CAN_BR_PHASE1( bt->phase1 - 1 )
				   | CAN_BR_PROPAG( bt->prog - 1 )
				   | CAN_BR_SJW( bt->sjw - 1 )
				   | CAN_BR_BRP( prescale - 1 );
	return 1;
}
