#include <slave/can.h>
#include <slave/state.h>
#include <soc/can.h>

#include <canard.h>
#include <stddef.h>
#include <stdint.h>


static void _mailbox_rx_handler( can_state_t * const s,
								 const size_t        m );

static bool _mailbox_tx_handler( can_state_t * const s,
								 const size_t        m );


void
#ifdef CONFIG_SAM3X8E_USE_CAN1
CAN1_Handler
#else
CAN0_Handler
#endif
( void )
{
	can_state_t     *can = &boulder->can;
	can_soc_state_t *soc = &can->soc;

	// проверка ящиков --------------------------------------------------
	
	uint32_t sr = CAN_IF->CAN_SR;
	uint8_t mbox_ix = SAM3X8E_CAN_MBOX_COUNT-1;
	uint8_t mbox_mask = CAN_SR_MB7;

	// сначала проходимся по ящикам для ресивера
	
	while ( mbox_ix >= CONFIG_SAM3X8E_CAN_TX_MBOX_COUNT ) {
		if ( sr & mbox_mask ) _mailbox_rx_handler( can, mbox_ix-- );
		mbox_mask >>= 1;
	}

	// и для трансивера

	uint8_t send_mask = 0;
	
	while ( mbox_mask ) {
		bool have_to_send = false;
		
		if ( sr & mbox_mask )
			have_to_send = _mailbox_tx_handler( can, mbox_ix-- );

		if ( have_to_send ) send_mask |= mbox_mask;
		
		mbox_mask >>= 1;
	}

	// сохраняем в контроллере маску для тех ящиков, из которых нужно
	// отправить фреймы -----------------------------------------------

	CAN_IF->CAN_TCR = send_mask;
	
	// подсчёт ошибок ----------------------------------------------

	const uint32_t error_mask = CAN_SR_ERRA | CAN_SR_BOFF
		                      | CAN_SR_SERR | CAN_SR_AERR
		                      | CAN_SR_FERR | CAN_SR_BERR;

	can->error_count += __builtin_popcount( sr & error_mask );
}


void
_mailbox_rx_handler( can_state_t * const s,
					 const size_t        m )
{
	can_message_t msg;
	CanMb *mbox = &CAN_IF->CAN_MB[ m ];	
	uint32_t mbox_status = mbox->CAN_MSR;
	
	msg.ts = mbox_status & CAN_MSR_MTIMESTAMP_Msk;

	// вытаскиваем кадр из ящика -----------------------------------

	msg.frame.id       = mbox->CAN_MID & 0x1FFFFFFFu;
	msg.frame.data_len = ( mbox_status & CAN_MSR_MDLC_Msk ) >> CAN_MSR_MDLC_Pos;
	
	*( (uint32_t *)  msg.frame.data )      = mbox->CAN_MDL;
	*( (uint32_t *) &msg.frame.data[ 4 ] ) = mbox->CAN_MDH;

	// сигналим, что ящик готов для приёма ------------------------

	mbox->CAN_MCR |= CAN_MCR_MTCR;

	// суём сообщение в очередь; canard его потом обработает --------

	slave_buffer_queue_push( &s->rx_queue, &msg, sizeof( can_message_t ) );
}


bool
_mailbox_tx_handler( can_state_t * const s,
					 const size_t        m )
{
	CanardCANFrame *frame = canardPeekTxQueue( &s->canard );

	if ( ! frame ) return false;
	
	CanMb *mbox = &CAN_IF->CAN_MB[ m ];	

	// копируем данные с кадра в почтовый ящик ---------------------------
		
	mbox->CAN_MID = frame->id | CAN_MID_MIDE;
	mbox->CAN_MCR = ( mbox->CAN_MCR & ~CAN_MCR_MDLC_Msk ) | CAN_MCR_MDLC( frame->data_len );
	mbox->CAN_MMR = mbox->CAN_MMR & ~CAN_MMR_PRIOR_Msk; // пока так
	mbox->CAN_MDL = *( (uint32_t *)  frame->data );
	mbox->CAN_MDH = *( (uint32_t *) &frame->data[ 4 ] );
	
	mbox->CAN_MSR &= ~CAN_MSR_MRTR;

	canardPopTxQueue( &s->canard );

	return true;
}
